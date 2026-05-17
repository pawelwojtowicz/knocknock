import knocknockHTTP
import cryptoTools

def test_simpleLogon_Success():
    # the test of the simpleDB logon authentication mehthod.
    # The authentication is successful if the user is found in the DB.
    # The auth method of the user3 is set up to simpleDB .
    data["userId"] = "user3"
    #data["password"] = "buongiorno$123"

    response = knocknockHTTP.login(data)

    assert response.status_code == 200
    assert "sessionId" in response.json()
