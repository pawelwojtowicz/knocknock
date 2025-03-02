#pragma once
#include <string>
#include <vector>

namespace knocknock
{
class CUserSetting
{
public:
  CUserSetting() = default;
  CUserSetting( const int appId, const std::string& paramName, const std::string& userId, const std::string& value);
  ~CUserSetting() = default;

  int GetApplicationId() const {
    return m_applicationId;
  }

  const std::string& GetUserId() const {
    return m_userId;
  }

  const std::string& GetParamName() const {
    return m_paramName;
  }

  void SetParamValue( const std::string& value) {
    m_value = value;
  }

  const std::string& GetParamValue() const {
    return m_value;
  }

private:
  int m_applicationId = {-1};
  std::string m_paramName = {};
  std::string m_userId = {};
  std::string m_value = {};
};

using tUserSettingsArray = std::vector<CUserSetting>;
}