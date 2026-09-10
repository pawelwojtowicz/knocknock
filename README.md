# knocknock

## Overview
The knocknock is an identity server that is dedicated for the embedded devices. The user list is stored in the SQLite database.

![image info](docu/img/knocknockOverview.png)

## Development
The server is implemented in the C++ with the intention to avoid newest C++ syntax, to make it possible to compile it with older C++ compilers.

### Installing the dependencies (on ubuntu)

```
$ sudo apt-get install nlohmann-json3-dev sqlite3 libsqlite3-dev openssl libssl-dev libboost1.83-all-dev libargon2-dev
``` 

### Building knocknock
The compilation of the knocknock is orchestrated with the use of the cmake - in the basic use case following steps bring fully functional software:

``` 
$ cd knocknock
$ mkdir build
$ cd build
$ cmake ..
$ make 
```
The ***knocknock*** requires two artifacts, that are instantiating the operational data: </br>
* configuration - the default file knocknock.conf is available in the ```config/knocknock.conf ```. The name and location of the configuration is provided as the only command line parameter with which we start the server.
* database - it includes the list of the users, along with the configuration of the authentication methods used by them, it also associates the roles and privileges to the users. The location of the database is specified in the configuration parameter (through the parameters: ```primaryDBLocation``` or ```secondaryDBLocation```).</br>
Example database that includes the example data is delivered in the source tree in the folder ```autotest/testData/testDB.db```
</br>

[//]: (close)
Starting up the server in the fresh environment, assuming the build procedure is completed:</br>
```
$ cd ..
$ ./build/src/knocknock/knocknock autoTest/testData/knocknock.conf
```

### Automated tests
The automated test procedures have been prepared with the use of the **pytest** .
The test cases ilustrate the typical communication sequences that implemented in the servers logic.

#### Configuring and running the autotests on Ubuntu

Ubuntu (24.04 and later) ships Python 3 by default, but blocks installing packages into the system Python directly (PEP 668) - a virtual environment is required. From the repo root:

```
$ sudo apt-get install python3-venv python3-pip
$ python3 -m venv autoTest/.venv
$ source autoTest/.venv/bin/activate
$ pip install -r autoTest/requirements.txt
```

With the venv still active, start the server (see "Starting up the server" above) in one terminal:
```
$ ./build/src/knocknock/knocknock autoTest/testData/knocknock.conf
```

Then, in another terminal (with `source autoTest/.venv/bin/activate` run again), execute the tests:
```
$ cd autoTest
$ pytest -v
```

Deactivate the virtual environment when done with `deactivate`. You do not need to recreate the venv or reinstall dependencies on subsequent runs - just re-activate it.

#### Running the autotests on Windows

Running the pytests in Windows 11 environment - assuming the user console is navigated to autoTest (and knocknock in the test configuration is running):</br>
```
$ py -m pytest
```

## Configuration

The configuration can be read from the configuration file and also from the database systemParameters table.

| Parameter name        | Type         | Description                     |
|:---| :---: | :--- |
| anonymousUserTemplate | string | Parameter specifies the userId of the template that will be used to create a anonymous user session. <br> If the parameter is equal to empty string - the anonumous login is dissallowed. |
| defaultAuthenticationMethod | string | The name of the user authentication method that is used in case there is not user specific method defined in the database <br> Supported authentication methods: <br> * checksum <br> * sha256 <br> * simpledb <br> * scr |
| defaultAuthenticationString | string | The default parameterization of the authentication method, the semanticsdo of the contents is specific to the authentication method determined for the user |
| allowedAuthMethods | string | Allowed Auth methods: <br> * argon2id <br> * checksum <br> * sha256 <br> * simpledb <br> * scr|
| sessionMaxAge | integer | |
| http.port | integer | HTTP listener port |
| http.cookieHttpOnly | bool | |
| http.cookiePath | string | "/"|
| http.cookieSecure| bool |false|
| http.cookieSameSite |[STRICT\|LAX\|NONE] | |
| maxLoginAttempts | integer | Maximum number of consecutive failed login attempts per userId before lockout. Default: 5 |
| loginLockoutSeconds | integer | Base lockout duration in seconds after exceeding max attempts. Doubles with each subsequent failure (exponential backoff), capped at 64x the base value. Default: 30 |
| sqlite3.SessionExpressPublisher.dbPath | string | Path to a dedicated SQLite file that active (VALID) sessions are published into, in a schema compatible with the `connect-sqlite3` store for the Node.js `express-session` middleware. Left empty (the default) disables the publisher entirely - it is opt-in. |

## Features
### HTTPS
    The support TLS/HTTPS shall be facilitated by utilizing the reverse proxies that can provide the TLS termination.

### Session publisher (Node.js / express-session integration)
When `sqlite3.SessionExpressPublisher.dbPath` is configured (see Configuration above), knocknock periodically publishes its currently active (`VALID`) sessions into a dedicated SQLite file, using the same table schema as the [`connect-sqlite3`](https://www.npmjs.com/package/connect-sqlite3) session store for the [`express-session`](https://www.npmjs.com/package/express-session) middleware. A Node.js/Express application can point `express-session` at that same file and read sessions created by knocknock directly, with no glue code of its own.

Install the two packages:
```
$ npm install express-session connect-sqlite3
```

Wire them up, pointing at the same file and table configured in knocknock's `sqlite3.SessionExpressPublisher.dbPath`:
```js
const express = require('express');
const session = require('express-session');
const SQLiteStore = require('connect-sqlite3')(session);

app.use(session({
  store: new SQLiteStore({
    db: 'sessionPublisher.db',   // filename part of sqlite3.SessionExpressPublisher.dbPath
    dir: 'autoTest/testData',    // directory part of sqlite3.SessionExpressPublisher.dbPath
    table: 'sessions',           // default table name used by the publisher
  }),
  secret: 'whatever',
  resave: false,
  saveUninitialized: false,
}));
```

Once configured, `req.session.userId`, `.userName`, `.state`, `.roles` and `.privileges` become directly readable on any request whose session cookie matches a session knocknock published. Sessions are removed from the table once they are logged out or expire (published sessions reflect the currently active set, not a permanent log). The password hash / authentication secret is deliberately never included in the published data.

