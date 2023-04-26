#include "ConfigParser.h"

ConfigParser::ConfigParser(std::string filePath) : _filePath(std::move(filePath))
{}

std::vector<std::pair<std::vector<threadContent>, std::vector<actionContent>>> ConfigParser::readConfig() try
{
    rapidjson::Document doc;
    doc.Parse(_filePath);

    if (doc.HasMember("TRTThread") && doc["TRTThread"].IsArray())
    {
        for (auto& object : doc["TRTThread"].GetArray())
        {

        }
    }
    else
    {
        throw std::runtime_error("Invalid config file");
    }
}
catch (...)
{
    throw;
}

void ConfigParser::writeConfig(const std::vector<std::pair<std::vector<threadContent>, std::vector<actionContent>>>& config)
{

}

