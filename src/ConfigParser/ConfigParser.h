#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H

#include "TRTIniFile.h"

#include <vector>
#include <iostream>


struct dataStructure
{
    std::string name;
    size_t count;
};


struct threadConfig
{
    std::string threadName;
    std::vector<dataStructure> threadContent;
};


class ConfigParser
{
public:
    ConfigParser(const std::string& path);

    std::vector<threadConfig> getConfig();
    void printConfig();

private:
    std::string _path;
};

#endif // CONFIGPARSER_H
