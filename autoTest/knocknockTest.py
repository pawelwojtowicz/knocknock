import knocknockHTTP
import cryptoTools

data = {}

data["userId"] = "chuck"
#data["password"] = "buongiorno$123"

response = knocknockHTTP.login(data)
returnData = response.json()

#print(returnData["userId"])
#sessionId = returnData["sessionId"]

#touchResponse = knocknockHTTP.touch({"sessionId": sessionId})
#print(touchResponse.status_code)

#logoutResponse = knocknockHTTP.logout({"sessionId": sessionId})
#print(logoutResponse.status_code)

#logoutResponse2 = knocknockHTTP.logout({"sessionId": sessionId})
#print(logoutResponse2.status_code)

sessionId = returnData["sessionId"]
challenge = returnData["challenge"]

print("Session ID: " + sessionId)
print("Challenge: " + challenge)

key = cryptoTools.generate_sha256("buongiorno$123")

challengeResponse = cryptoTools.aes_cbc_encrypt(challenge,  sessionId, key)
print(challengeResponse)

authRequestData = {
    "sessionId": sessionId,
    "challenge_response": challengeResponse
}

authResponse = knocknockHTTP.auth(authRequestData)
print(authResponse.text)
print(authResponse.status_code) 