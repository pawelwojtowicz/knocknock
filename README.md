# knocknock

## Overview
The knocknock is an identity server that is dedicated for the embedded devices. The user list is stored in the SQLite database.

## Development
The server is implemented in the C++ with the intention to avoid newest C++ syntax, to make it possible to compile it with older C++ compilers.

### Installing the dependencies (on ubuntu)

`sudo apt-get install sqlite3 libsqlite3-dev libcrypto++-dev libcrypto++-doc libcrypto++-utils`

## Configuration

The configuration can be read from the configuration file and also from the database systemParameters table.

| Parameter name        | Type         | Description                     |
|-----------------------|--------------|---------------------------------|
| allowAnonymousSession | [true\|false]| parameter specifies whether it is possible to create session for a user that is not defined in the database |
| defaultAuthenticationMethod | string | The name of the user authentication method that is used in case there is not user specific method defined in the database <br> Supported authentication methods: <br> * checksum <br> * OTP <br>* SHA256|
