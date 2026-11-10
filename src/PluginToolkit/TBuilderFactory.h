#pragma once
#include <string>
#include <map>
#include <memory>
#include <functional>

namespace knocknock
{

template <typename T>
class TBuilderFactory
{
public:
    using tClassPtr = std::unique_ptr<T>;
    using tCreationMethod = std::function<tClassPtr()>;
    using tCreationMethodMap = std::map<std::string, tCreationMethod>;

    TBuilderFactory() = default ;
    virtual ~TBuilderFactory() = default;

    tClassPtr Create( const std::string& className ) const
    {
        const auto iter = m_creationMap.find(className);
        if ( m_creationMap.end() != iter )
        {
            return iter->second();
        } 

        return {};
    }

    bool RegisterBuilder( const std::string& name, tCreationMethod builderMethod)
    {
        auto iter = m_creationMap.find(name);
        if ( m_creationMap.end() == iter )
        {
            m_creationMap.emplace( name, std::move(builderMethod) );
            return true;
        }
        return false;
    }

private:
    tCreationMethodMap m_creationMap;
};

}