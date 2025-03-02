#pragma once
#include <string>
#include <vector>

namespace knocknock
{
class CApplicationParam
{
public:
  CApplicationParam() = default;
  CApplicationParam( const int applicationId, const std::string& paramName, bool isPublic, const std::string& value);
  ~CApplicationParam() = default;

  void SetPublicFlag( const bool isPublic ) {
    m_public = isPublic;
  }

  void SetParamName( const std::string& name ) {
    m_paramName = name;
  }

  void SetValue( const std::string& value) {
    m_value = value;
  }

  int GetApplicationId() const {
    return m_applicationId;
  }

  const std::string& GetParameterName() const {
    return m_paramName;
  }

  const bool IsPublic() const {
    return m_public;
  }

  const std::string& GetValue() const {
    return m_value;
  }

private:
  int m_applicationId = {-1};
  std::string m_paramName = {};

  bool m_public = {};
  std::string m_value = {};
};

using tApplicationParamsArray = std::vector<CApplicationParam>;
}