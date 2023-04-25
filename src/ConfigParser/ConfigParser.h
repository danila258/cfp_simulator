#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H

#include <string>
#include <vector>


struct objectFormat
{
    std::string className;
    std::string varName;
    int count;
    std::vector<std::string> args;
};

struct threadFormat
{
    int number;
    std::vector<objectFormat> objects;
};

struct actionFormat
{
    std::string action;
    std::string className;
    std::string varName;
    std::vector<std::string> args;
};

class ConfigParser
{

};


#endif //CONFIGPARSER_H
