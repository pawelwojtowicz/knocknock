import knocknockHTTP
import cryptoTools
import time

def test_simpleDB_Logon_Success():
    # the test of the simpleDB logon authentication mehthod.
    # The authentication is successful if the user is found in the DB.
    # The auth method of the user3 is set up to simpleDB .
    # Further the touch mechanism is tested. The session shall be 
    # invalidated after three seconds (test configuration)
    data = {}
    data["userId"] = "user3"
    #data["password"] = "buongiorno$123"

    response = knocknockHTTP.login(data)

    assert response.status_code == 200
    assert "sessionId" in response.json()

    sessionId = response.json()["sessionId"]

    touchRequestData = {}
    touchRequestData["sessionId"] = sessionId
    touchResponse = knocknockHTTP.touch(touchRequestData)
    assert touchResponse.status_code == 200
    assert "userId" in touchResponse.json()
    assert touchResponse.json()["userId"] == "user3"

    time.sleep(4)
    touchResponse2 = knocknockHTTP.touch(touchRequestData)
    assert touchResponse2.status_code == 401
    assert "userId" not in touchResponse2.json()
    
def test_simpleDB_Logon_Failure():
    # the test of the simpleDB logon authentication mehthod.
    # The authentication is unsuccessful if the user is not found in the DB.

    data = {}
    data["userId"] = "herrFlick"

    response = knocknockHTTP.login(data)

    assert response.status_code == 401

def test_simpleDB_LogonWithoutPassword_Failure():
    # the test of the user which auth method is set up as sha256.
    # The authentication is unsuccessful if there is not valid password provided

    data = {}
    data["userId"] = "user1"

    response = knocknockHTTP.login(data)

    #ToDo - reafactor the code - revealing too much information
    assert response.status_code == 500

def test_sha256_Logon_Success():
    # the test of the sha256 logon authentication mehthod.
    # The authentication is successful if the user is found in the DB and the password is correct.
    # The auth method of the user1 is set up to sha256 .

    data = {}
    data["userId"] = "user1"
    data["password"] = "buongiorno$123"

    response = knocknockHTTP.login(data)

    assert response.status_code == 200
    assert "sessionId" in response.json()

def test_sha256_Logon_Failure():
    # the test of the sha256 logon authentication mehthod.
    # The authentication is unsuccessful if the user is not found in the DB or the password is incorrect.
    # The auth method of the user1 is set up to sha256 .

    data = {}
    data["userId"] = "user1"
    data["password"] = "buonanotte$123"

    response = knocknockHTTP.login(data)

    assert response.status_code == 401

def test_scr_Logon_Success():
    # the scr - simple-challenge-response auth method is a subject of the test
    # The login request returns the challenge and the sessionId, which are used
    # along with the password to generate the challenge response for the auth request

    loginData = {}
    loginData["userId"] = "chuck"

    loginResponse = knocknockHTTP.login(loginData)

    assert loginResponse.status_code == 201
    assert not ( "userId"  in loginResponse.json() )
    assert "challenge" in loginResponse.json()

    challenge = loginResponse.json()["challenge"]

    # prepare the challenge response
    encryptionKey = cryptoTools.generate_sha256("buongiorno$123")
    encryptionIV = loginResponse.json()["sessionId"]
    challengeResponse = cryptoTools.aes_cbc_encrypt(challenge, encryptionIV, encryptionKey)

    authData = {}
    authData["sessionId"] = loginResponse.json()["sessionId"]
    authData["challenge_response"] = challengeResponse

    authResponse = knocknockHTTP.auth(authData)

    assert authResponse.status_code == 200
    assert authResponse.json()["userId"] == "chuck"
    assert authResponse.json()["userName"] == "Chuck Norris"
    assert authResponse.json()["sessionId"] == loginResponse.json()["sessionId"]

def test_scr_Logon_Failure_AuthTooLate():
    # the scr - simple-challenge-response auth method is a subject of the test
    # The login request returns the challenge and the sessionId.
    # The timespan beteween the login and auth request is longer than the allowed timespan for 
    # the auth request processing (test configuration)
    loginData = {}
    loginData["userId"] = "chuck"

    loginResponse = knocknockHTTP.login(loginData)

    assert loginResponse.status_code == 201
    assert not ( "userId"  in loginResponse.json() )
    assert "challenge" in loginResponse.json()

    challenge = loginResponse.json()["challenge"]

    # prepare the challenge response
    encryptionKey = cryptoTools.generate_sha256("buongiorno$123")
    encryptionIV = loginResponse.json()["sessionId"]
    challengeResponse = cryptoTools.aes_cbc_encrypt(challenge, encryptionIV, encryptionKey)

    authData = {}
    authData["sessionId"] = loginResponse.json()["sessionId"]
    authData["challenge_response"] = challengeResponse

    time.sleep(4)
    authResponse = knocknockHTTP.auth(authData)

    assert authResponse.status_code == 401
    assert not ("userId" in authResponse.json())
    assert not ("userName" in authResponse.json())
    assert not ("sessionId" in authResponse.json())

def test_scr_Logon_Failure_WrongPassword():
    # the scr - simple-challenge-response auth method is a subject of the test
    # The login request returns the challenge and the sessionId.
    # The timespan beteween the login and auth request is longer than the allowed timespan for 
    # the auth request processing (test configuration)
    loginData = {}
    loginData["userId"] = "chuck"

    loginResponse = knocknockHTTP.login(loginData)

    assert loginResponse.status_code == 201
    assert not ( "userId"  in loginResponse.json() )
    assert "challenge" in loginResponse.json()

    challenge = loginResponse.json()["challenge"]

    # prepare the challenge response
    encryptionKey = cryptoTools.generate_sha256("buonanotte$123")
    encryptionIV = loginResponse.json()["sessionId"]
    challengeResponse = cryptoTools.aes_cbc_encrypt(challenge, encryptionIV, encryptionKey)

    authData = {}
    authData["sessionId"] = loginResponse.json()["sessionId"]
    authData["challenge_response"] = challengeResponse

    authResponse = knocknockHTTP.auth(authData)

    assert authResponse.status_code == 401
    assert not ("userId" in authResponse.json())
    assert not ("userName" in authResponse.json())
    assert not ("sessionId" in authResponse.json())   