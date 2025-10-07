#pragma once
#include <string>
#include <map>
namespace HTTPServer
{

struct URLInfo
{
  std::string path;
  std::string user;
  std::string password;

  std::map<std::string, std::string> parameters;
};
}