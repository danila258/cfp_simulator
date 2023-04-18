#include "MainLogic.h"
#include "ObjectFactory.h"

#include <memory>

int main(int argc, char* argv[])
{
    MainLogic logic(argc, argv);

    ObjectFactory factory;
    factory.registerType<TRTMutexObject>();
    std::vector<std::string> arr;
    std::string s = "TRTMutex";
    auto t = factory.create(s, arr);
    auto* v = t.get();

    return 0;
}
