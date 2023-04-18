#include "ObjectFactory.h"

std::unique_ptr<Object> ObjectFactory::create(const std::string& className, const std::vector<std::string>& args)
{
    auto it = _typeMap.find(className);

    if (it == _typeMap.end())
    {
        throw std::runtime_error("unknown type");
    }

    return (it->second)(args);
}
