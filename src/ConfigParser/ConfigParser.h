#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H

#include "../lib/IniFile/IniFile.h"
#include "../WorkerThreads/CustomMutex.h"
#include "../WorkerThreads/CustomQueue.h"

#include <vector>
#include <array>
#include <string>
#include <unordered_map>
#include <fstream>
#include <algorithm>
#include <iostream>


namespace parser
{
    constexpr auto separatorInSection = '_';
    constexpr auto mutexKey = "mutex";
    constexpr auto queueKey = "que";
    constexpr auto timerKey = "timer";
    constexpr auto semaphoreKey = "sem";
    constexpr auto countKey = "count";
    constexpr auto sizeKey = "size";
    constexpr auto lockKey = "lock";
    constexpr auto fullKey = "isFull";
    constexpr auto emptyKey = "isEmpty";
    constexpr auto stateKey = "state";

    constexpr auto threadSection = "thread";
    constexpr auto mutexSection = "mutex";
    constexpr auto timerSection = "timer";
    constexpr auto semaphoreSection = "sem";
    constexpr auto queueSection = "que";

    // the first element must be threadSection because we can't check other sections without known threads numbers
    constexpr std::array<const char*, 5> sections = {threadSection, mutexSection, queueSection, timerSection,
                                                     semaphoreSection};

    constexpr std::array<const char*, 10> allKeys = {mutexKey, queueKey, timerKey, semaphoreKey, countKey, sizeKey,
                                                    lockKey, fullKey, emptyKey, stateKey};
}


struct dataContainers
{
    size_t mutexCount;
    size_t queueCount;
    std::vector<mutexParams> mutexes;
    std::vector<queueParams> queue;
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
                    {parser::queueSection, &ConfigParser::readQueue}};

    static void getNameNumber(const IniSection& section, std::string& name, int& number);
    static std::string getKeyErrorMessage(const std::string& key, const IniSection& section);

    void readThread(const IniSection& section, int threadNum);
    void readMutex(const IniSection& section, int threadNum);
    void readQueue(const IniSection& section, int threadNum);
    void readTimer(const IniSection& section, int threadNum);
    void readSemaphore(const IniSection& section, int threadNum);
};


#endif // CONFIGPARSER_H
