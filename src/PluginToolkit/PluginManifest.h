#pragma once
#include <memory>
#include "IKnocknockPlugin.h"

#define DEFINE_PLUGIN_ROOT(rootClassName) \
extern "C" \
{ \
std::unique_ptr<knocknock::IKnocknockPlugin> createInstance() \
{ \
  return std::make_unique<rootClassName>(); \
} \
}
