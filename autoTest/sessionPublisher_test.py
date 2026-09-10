import json
import sqlite3
import time
from pathlib import Path

import knocknockHTTP
import cryptoTools

# Must match testData/knocknock.conf's sqlite3.SessionExpressPublisher.dbPath,
# resolved relative to this file rather than the current working directory,
# since the server (run from the repo root per the README) and pytest (run
# from autoTest/) don't share a working directory.
SESSION_PUBLISHER_DB_PATH = Path(__file__).parent / "testData" / "sessionPublisher.db"


def _read_published_session(sessionId, timeoutSeconds=2.0, pollIntervalSeconds=0.02):
    """Poll the session publisher's SQLite file for a row with the given sid.

    PublishSessions() only hands the snapshot off to CSessionPublisher's
    background thread - the actual SQLite write follows asynchronously a few
    milliseconds later - so this polls briefly rather than reading once.
    Returns (sess_dict, expired, expired_sqlite_type) or (None, None, None)
    if no row shows up within the timeout.
    """
    deadline = time.time() + timeoutSeconds
    while time.time() < deadline:
        if SESSION_PUBLISHER_DB_PATH.exists():
            conn = sqlite3.connect(str(SESSION_PUBLISHER_DB_PATH))
            try:
                row = conn.execute(
                    "SELECT sess, expired, typeof(expired) FROM sessions WHERE sid = ?",
                    (sessionId,),
                ).fetchone()
            finally:
                conn.close()
            if row is not None:
                sess, expired, expiredType = row
                return json.loads(sess), expired, expiredType
        time.sleep(pollIntervalSeconds)
    return None, None, None


def _session_row_exists(sessionId, timeoutSeconds=0.3):
    sess, _, _ = _read_published_session(sessionId, timeoutSeconds=timeoutSeconds, pollIntervalSeconds=0.05)
    return sess is not None


def test_sha256_ValidSession_IsPublishedImmediately():
    # sha256 reaches VALID directly in Login() (single-step auth), so the
    # session must show up in the publisher DB right away - no need to wait
    # for the periodic Tick().
    data = {"userId": "user1", "password": "buongiorno$123"}
    response = knocknockHTTP.login(data)
    assert response.status_code == 200
    sessionId = response.json()["sessionId"]

    sess, expired, expiredType = _read_published_session(sessionId)

    assert sess is not None, "session was not published"
    assert sess["userId"] == "user1"
    assert sess["state"] == "VALID"
    assert isinstance(sess["roles"], list)
    assert isinstance(sess["privileges"], list)
    assert "cookie" in sess
    assert "expires" in sess["cookie"]

    # `expired` must be a real SQLite INTEGER (unix milliseconds), not TEXT,
    # or a connect-sqlite3 client's numeric "? <= expired" comparison would
    # never match.
    assert expiredType == "integer"
    assert expired > time.time() * 1000


def test_scr_ValidSession_IsPublishedOnlyAfterAuth():
    # scr only reaches VALID on the /auth step (CSessionManager::Authenticate()),
    # not on /login - the row must not appear before that, and must appear
    # right after /auth succeeds without waiting for a Tick().
    loginResponse = knocknockHTTP.login({"userId": "chuck"})
    assert loginResponse.status_code == 201
    sessionId = loginResponse.json()["sessionId"]
    challenge = loginResponse.json()["challenge"]

    # AUTH_IN_PROGRESS is not "eligible" yet - must not be published.
    assert not _session_row_exists(sessionId)

    encryptionKey = cryptoTools.generate_sha256("buongiorno$123")
    challengeResponse = cryptoTools.aes_cbc_encrypt(challenge, sessionId, encryptionKey)
    authResponse = knocknockHTTP.auth({
        "sessionId": sessionId,
        "challenge_response": challengeResponse,
    })
    assert authResponse.status_code == 200

    sess, expired, expiredType = _read_published_session(sessionId)

    assert sess is not None, "session was not published after /auth succeeded"
    assert sess["userId"] == "chuck"
    assert sess["userName"] == "Chuck Norris"
    assert sess["state"] == "VALID"
    assert "KARATE_MASTER" in sess["roles"]
    assert expiredType == "integer"


def test_scr_FailedAuth_IsNeverPublished():
    # a session that never reaches VALID (wrong challenge response) must
    # never appear in the publisher table.
    loginResponse = knocknockHTTP.login({"userId": "chuck"})
    assert loginResponse.status_code == 201
    sessionId = loginResponse.json()["sessionId"]
    challenge = loginResponse.json()["challenge"]

    wrongKey = cryptoTools.generate_sha256("wrongPassword$1")
    challengeResponse = cryptoTools.aes_cbc_encrypt(challenge, sessionId, wrongKey)
    authResponse = knocknockHTTP.auth({
        "sessionId": sessionId,
        "challenge_response": challengeResponse,
    })
    assert authResponse.status_code == 401

    assert not _session_row_exists(sessionId)


def test_LoggedOutSession_IsRemovedFromPublisher():
    # once logged out, a session is no longer "active" and must be dropped
    # from the publisher table - this only happens on the next Tick()
    # (roughly 1s cadence), not immediately, so this polls rather than
    # asserting right after the /logout response.
    data = {"userId": "user1", "password": "buongiorno$123"}
    response = knocknockHTTP.login(data)
    assert response.status_code == 200
    sessionId = response.json()["sessionId"]

    assert _session_row_exists(sessionId), "precondition failed: session was never published"

    logoutResponse = knocknockHTTP.logout({"sessionId": sessionId})
    assert logoutResponse.status_code == 200

    deadline = time.time() + 3.0
    removed = False
    while time.time() < deadline:
        if not _session_row_exists(sessionId, timeoutSeconds=0.1):
            removed = True
            break
        time.sleep(0.1)

    assert removed, "logged-out session was not removed from the publisher table"


def test_ExpiredSession_IsRemovedFromPublisher():
    # testData/knocknock.conf sets sessionExpirationTimeout=3 - once a
    # session's expiry passes, CleanupExpiredSessions() purges it and the
    # next Tick()'s republish must no longer include it.
    data = {"userId": "user1", "password": "buongiorno$123"}
    response = knocknockHTTP.login(data)
    assert response.status_code == 200
    sessionId = response.json()["sessionId"]

    assert _session_row_exists(sessionId), "precondition failed: session was never published"

    time.sleep(4)  # past the 3s expiration configured for this test environment

    deadline = time.time() + 3.0
    removed = False
    while time.time() < deadline:
        if not _session_row_exists(sessionId, timeoutSeconds=0.1):
            removed = True
            break
        time.sleep(0.1)

    assert removed, "expired session was not removed from the publisher table"
