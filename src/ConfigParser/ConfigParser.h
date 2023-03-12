#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H

#include "TRTIniFile.h"
#include "customMutex.h"
#include "customQueue.h"

#include <vector>
#include <array>
#include <string>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <algorithm>


namespace parser
{
    constexpr auto fileCopyName = "copy.txt";

    constexpr auto separatorInSection = '_';
    constexpr auto mutexKey = "mutex";
    constexpr auto queueKey = "que";
    constexpr auto countKey = "count";
    constexpr auto sizeKey = "size";
    constexpr auto lockKey = "lock";
    constexpr auto fullKey = "isFull";
    constexpr auto emptyKey = "isEmpty";

    constexpr auto threadSection = "thread";
    constexpr auto mutexSection = "mutex";
    constexpr auto queueSection = "que";

    // the first element must be threadSection because we can't check other sections without known threads numbers
    constexpr std::array<const char*, 3> sections = {threadSection, mutexSection, queueSection};
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
    void printConfig();

private:
    std::string _path;
    std::vector<threadConfig> _config;

    const std::unordered_map<std::string, std::function<void(ConfigParser*, TRTIniFile&, std::string&, int)>>
    _functionMap = {{parser::threadSection, &ConfigParser::readThread},
                   {parser::mutexSection,   &ConfigParser::readMutex},
                   {parser::queueSection, &ConfigParser::readQueue}};

    std::string createFileCopy();

    static void getNameNumber(const std::string& section, std::string& name, int& number);
    static std::string getKeyErrorMessage(const std::string& key, const std::string& section);

    void readThread(TRTIniFile& file, std::string& section, int number);
    void readMutex(TRTIniFile& file, std::string& section, int number);
    void readQueue(TRTIniFile& file, std::string& section, int number);
};

#endif // CONFIGPARSER_H
