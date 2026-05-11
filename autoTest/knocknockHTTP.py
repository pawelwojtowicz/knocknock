import httpx

ipAddress = "127.0.0.1"
port = "8081"


def login(data):
    response = httpx.post(f"http://{ipAddress}:{port}/login", json=data)
    return response

def auth(data):
    response = httpx.post(f"http://{ipAddress}:{port}/auth", json=data)
    return response

def logout(data):
    response = httpx.post(f"http://{ipAddress}:{port}/logout", json=data)
    return response

def touch(data):
    response = httpx.post(f"http://{ipAddress}:{port}/touch", json=data)
    return response