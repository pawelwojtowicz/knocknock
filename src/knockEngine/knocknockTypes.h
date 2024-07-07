#pragma once 

enum class UserSessionState
{
  CREATED,        //created a session for an endpoint ( with UserId )
  AUTHENTICATED,  //authentication challenged passed
  TIMED_OUT,      //next request requires the
  STALLED
};