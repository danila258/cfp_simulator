#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H

#include "IniFile.h"

#include <vector>
#include <array>
#include <string>
#include <unordered_map>
#include <fstream>
#include <algorithm>
#include <functional>


namespace parser
{
    constexpr auto separatorInSection = '_';
    constexpr auto mutexKey = "mutex";
    constexpr auto semaphoreKey = "sem";
    constexpr auto queueKey = "que";
    constexpr auto timerKey = "timer";
    constexpr auto eventKey = "event";
    constexpr auto condVarKey = "condvar";
    constexpr auto memoryKey = "memory";
    constexpr auto countKey = "count";
    constexpr auto sizeKey = "size";
    constexpr auto lockKey = "lock";
    constexpr auto fullKey = "isFull";
    constexpr auto emptyKey = "isEmpty";
    constexpr auto stateKey = "state";
    constexpr auto timeoutKey = "timeout";
    constexpr auto fStateKey = "fState";
    constexpr auto lSizeKey = "lSize";
    constexpr auto bMapKey = "bMap";

    constexpr auto threadSection = "thread";
    constexpr auto mutexSection = "mutex";
    constexpr auto semaphoreSection = "sem";
    constexpr auto queueSection = "que";
    constexpr auto timerSection = "timer";
    constexpr auto eventSection = "event";
    constexpr auto condVarSection = "condvar";
    constexpr auto memorySection = "memory";


    // the first element must be threadSection because we can't check other sections without known threads numbers
    constexpr std::array<const char*, 5> sections = {threadSection, mutexSection, queueSection, timerSection,
                                                     semaphoreSection};

    constexpr std::array<const char*, 10> allKeys = {mutexKey, queueKey, timerKey, semaphoreKey, countKey, sizeKey,
                                                    lockKey, fullKey, emptyKey, stateKey};
}


struct mutexParams
{
    int id;
    std::string name = "mutex";
    bool lock;
};


struct queueParams
{
    int id;
    std::string name = "queue";
    size_t size;
    bool isFull;
    bool isEmpty;
};


struct semaphoreParams
{
    int id;
    std::string name = "sem";
    bool state;
};


struct timerParams
{
    int id;
    std::string name = "timer";
    size_t timeout;
    bool state;
};

struct eventParams
{
    int id;
    std::string name = "event";
    bool fState;
};


struct condvarParams
{
    int id;
    std::string name = "condvar";
};


struct memoryParams
{
    int id;
    std::string name = "memory";
    size_t lSize;
    bool bMap;
    size_t timeout;
};


struct dataContainers
{
    size_t mutexCount;
    size_t semaphoreCount;
    size_t queueCount;
    size_t timerCount;
    size_t eventCount;
    size_t condvarCount;
    size_t memoryCount;

    std::vector<mutexParams> mutexes;
    std::vector<semaphoreParams> semaphores;
    std::vector<queueParams> queues;
    std::vector<timerParams> timers;
    std::vector<eventParams> events;
    std::vector<condvarParams> condvars;
    std::vector<memoryParams> memory;
};


struct threadConfig
{    
    int number;
    dataContainers config;
};


class ConfigParser
{
public:
    ConfigParser(const std::string& path);

    std::vector<threadConfig> getConfig();

private:
    IniFile _file;
    std::vector<threadConfig> _config;

    // defines a specific parsing function for the specific section name
    const std::unordered_map<std::string, std::function<void(ConfigParser*, const IniSection&, int threadNum)>>
    _functionMap = {{parser::threadSection, &ConfigParser::readThread},
                    {parser::mutexSection, &ConfigParser::readMutex},
                    {parser::semaphoreSection, &ConfigParser::readSemaphore},
                    {parser::queueSection, &ConfigParser::readQueue},
                    {parser::timerSection, &ConfigParser::readTimer}};

    static void getNameNumber(const IniSection& section, std::string& name, int& number);
    void keyExists(const IniSection& section, const std::string& key);
    size_t getThreadIndex(const IniSection& section, int threadNum);

    void readThread(const IniSection& section, int threadNum);
    void readMutex(const IniSection& section, int threadNum);
    void readSemaphore(const IniSection& section, int threadNum);
    void readQueue(const IniSection& section, int threadNum);
    void readTimer(const IniSection& section, int threadNum);
    void readEvent(const IniSection& section, int threadNum);
    void readCondvar(const IniSection& section, int threadNum);
    void readMemory(const IniSection& section, int threadNum);
};


#endif // CONFIGPARSER_H
