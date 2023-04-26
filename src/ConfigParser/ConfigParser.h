#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H

#include <string>
#include <vector>
#include <utility>

#include "rapidjson/document.h"


struct objectContent
{
    std::string className;
    std::string varName;
    int count;
    std::vector<std::string> args;
};

struct threadContent
{
    int number;
    std::vector<objectContent> objects;
};

struct actionContent
{
    std::string action;
    objectContent object;
};


class ConfigParser
{
public:
    ConfigParser(std::string filePath);

    std::vector<std::pair<std::vector<threadContent>, std::vector<actionContent>>> readConfig();
    void writeConfig(const std::vector<std::pair<std::vector<threadContent>, std::vector<actionContent>>>& config);

private:
    std::string _filePath;
};


#endif //CONFIGPARSER_H
