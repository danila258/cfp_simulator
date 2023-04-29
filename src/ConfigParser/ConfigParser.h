#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H

#include "UniversalString.h"
#include "DataTransferInterface.h"

#include "rapidjson/include/rapidjson/document.h"
#include "rapidjson/include/rapidjson/ostreamwrapper.h"
#include "rapidjson/include/rapidjson/prettywriter.h"

#include <string>
#include <vector>
#include <fstream>
#include <sstream>


class ConfigParser
{
public:
    explicit ConfigParser(std::string filePath);

    std::vector<threadContent> getThreads();
    std::vector<actionContent> getActions();

    void writeConfig(const std::vector<threadContent>& threads, const std::vector<actionContent>& actions);

private:
    std::string _filePath;

    rapidjson::Document getParsedDocument();
    static rapidjson::Value getValue(const UniversalString& str, rapidjson::Document::AllocatorType& allocator);
};


#endif //CONFIGPARSER_H
