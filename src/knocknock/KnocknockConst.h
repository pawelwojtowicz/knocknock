#pragma once

namespace knocknock
{
  constexpr char cParamNamePrimaryDBLocation[] = "primaryDBLocation";
  constexpr char cParamNameSecondaryDBLocation[] = "secondaryDBLocation";
  constexpr char cDefaultDBLocation[] = "knocknock.db";

  constexpr char cParamNameHTTPCookieHttpOnly[] = "http.cookieHttpOnly";
  constexpr char cParamNameHTTPCookiePath[] = "http.cookiePath";
  constexpr char cParamNameHTTPCookieSecure[] = "http.cookieSecure";
  constexpr char cParamNameHTTPCookieSameSite[] = "http.cookieSameSite";

  constexpr char cParamNameHTTPServerPort[] = "httpServer.port";
  constexpr char cParamNameHTTPServerThreadPoolSize[] = "httpServer.threadPoolSize";

  constexpr int cHTTPResult_OK = 200;
  constexpr int cHTTPResult_Created = 201;
  constexpr int cHTTPResult_BadRequest = 400;
  constexpr int cHTTPResult_Unauthorized = 401;
  constexpr int cHTTPResult_Forbidden = 403;
  constexpr int cHTTPResult_NotFound = 404;
  constexpr int cHTTPResult_InternalServerError = 500;
}