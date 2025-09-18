# knocknock

## Overview
The knocknock is an identity server that is dedicated for the embedded devices. The user list is stored in the SQLite database.

![image info](docu/img/knocknockOverview.png)

## Development
The server is implemented in the C++ with the intention to avoid newest C++ syntax, to make it possible to compile it with older C++ compilers.

### Installing the dependencies (on ubuntu)

`sudo apt-get install sqlite3 libsqlite3-dev libcrypto++-dev libcrypto++-doc libcrypto++-utils`

## Configuration

The configuration can be read from the configuration file and also from the database systemParameters table.

| Parameter name        | Type         | Description                     |
|:---| :---: | :--- |
| anonymousUserTemplate | string | Parameter specifies the userId of the template that will be used to create a anonymous user session. <br> If the parameter is equal to empty string - the anonumous login is dissallowed. |
| defaultAuthenticationMethod | string | The name of the user authentication method that is used in case there is not user specific method defined in the database <br> Supported authentication methods: <br> * checksum <br> * sha256 <br> * simpledb <br> * scr |
| defaultAuthenticationString | string | The default parameterization of the authentication method, the semanticsdo of the contents is specific to the authentication method determined for the user |
| allowedAuthMethods | string | Allowed Auth methods: <br> * checksum <br> * sha256 <br> * simpledb <br> * scr |
| sessionMaxAge | integer | |
| cookieHttpOnly | bool | |
| cookiePath | string | "/"|
| cookieSecure| bool |false|
| cookieSameSite |[STRICT\|LAX\|NONE] | |

