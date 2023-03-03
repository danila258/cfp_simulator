#include "ConfigParser.h"

ConfigParser::ConfigParser(const std::string& path) : _path(path)
{}

std::vector<threadConfig> ConfigParser::getConfig()
{
    TRTIniFile file(_path, true);
    std::vector<threadConfig> config;

    if ( !file.FileExists(_path) )
    {
        throw std::runtime_error("config file not exist at path: " + _path);
    }

    if ( !file.CanRead(_path) )
    {
        throw std::runtime_error("no permission to read config file");
    }

    std::vector<std::string> sections = file.GetSections();
    config.reserve(sections.size());

    for (auto& section : sections)
    {
        std::vector<std::string> keys = file.GetKeys(section);
        std::vector<std::string> values = file.GetValues(section);

        std::vector<dataStructure> threadContent;
        threadContent.reserve(keys.size());

        if (keys.size() != values.size())
        {
            throw std::runtime_error("keys size is not equal to values size");
        }

        try
        {
            for (size_t i = 0; i < keys.size(); ++i)
            {
                threadContent.push_back({keys[i], std::stoull(values[i])});
            }
        }
        catch (...)
        {
            throw std::runtime_error("not valid value in section: " + section);
        }

        config.push_back({section, std::move(threadContent)});
    }

    return config;
}

void ConfigParser::printConfig()
{
    std::ios_base::sync_with_stdio(false);

    auto config = getConfig();

    for (auto& section : config)
    {
        std::cout << section.threadName << '\n';

        for (auto&& keyVal : section.threadContent)
        {
            std::cout << keyVal.name << " = " << keyVal.count << '\n';
        }

        std::cout << '\n';
    }
}
