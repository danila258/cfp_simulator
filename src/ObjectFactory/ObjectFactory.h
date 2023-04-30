#ifndef OBJECTFACTORY_H
#define OBJECTFACTORY_H

#include "UniversalString.h"

#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>
#include <stdexcept>


class Object
{
public:
    virtual void call(const UniversalString& funcName, const std::vector<UniversalString>& args) = 0;
    virtual ~Object() = default;

protected:
    std::unique_ptr<void, std::function<void(void*)>> _object;
};


class ObjectFactory
{
public:
    std::unique_ptr<Object> create(const UniversalString& className, const std::vector<UniversalString>& args);

    template<class T>
    void registerType();

private:
    std::unordered_map<std::string, std::function<std::unique_ptr<Object>(const std::vector<UniversalString>&)>> _typeMap;
};

template<class T>
void ObjectFactory::registerType()
{
    _typeMap.emplace(T::staticTypeName(), [](const std::vector<UniversalString>& args) -> std::unique_ptr<Object> {
        return std::unique_ptr<Object>(dynamic_cast<Object*>(new T(args)));
    });
}


#endif //OBJECTFACTORY_H