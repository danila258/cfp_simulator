#ifndef OBJECTFACTORY_H
#define OBJECTFACTORY_H

#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>
#include <stdexcept>


class Object
{
public:
    virtual void call(const std::string& funcName, const std::vector<std::string>& args) = 0;
    virtual ~Object() = default;

protected:
    std::unique_ptr<void, std::function<void(void*)>> _object;
};


class ObjectFactory
{
public:
    std::unique_ptr<Object> create(const std::string& className, const std::vector<std::string>& args);

    template<class T>
    void registerType();

private:
    std::unordered_map<std::string, std::function<std::unique_ptr<Object>(const std::vector<std::string>&)>> _typeMap;
};

template<class T>
void ObjectFactory::registerType()
{
    _typeMap.emplace(T::staticTypeName(), [](const std::vector<std::string>& args) -> std::unique_ptr<Object> {
        return std::unique_ptr<Object>(dynamic_cast<Object*>(new T(args)));
    });
}


#endif //OBJECTFACTORY_H
