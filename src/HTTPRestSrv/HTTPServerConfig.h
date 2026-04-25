#pragma once

namespace HTTPServer
{

struct HTTPServerConfig
{
  short unsigned int port = { 8080 };
  unsigned int threadPoolSize = { 4 };
};
}