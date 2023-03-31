#include "ConfigParser.h"

ConfigParser::ConfigParser(const std::string& path) : _file(path)
{}

std::vector<threadConfig> ConfigParser::getConfig()
{
    try
    {
        _file.load();
    }
    catch (...)
    {
        throw;
    }

    _config.clear();

    std::string name;
    int threadNum;

    for (auto& knownSection : parser::sections)
    {
        for (auto& section : _file.sections())
        {
            try
            {
                getNameNumber(section, name, threadNum);

                if (name == knownSection)
                {
                    (_functionMap.find(name)->second)(this, std::ref(section), threadNum);
                }
                else if (std::find(parser::sections.begin(), parser::sections.end(), name) == parser::sections.end())
                {
                    throw std::runtime_error("unknown section in line: " + std::to_string(section.getLineNum()));
                }
            }
            catch (...)
            {
                throw;
            }
        }
    }

    for (auto& thread : _config)
    {
        if (thread.config.mutexCount != thread.config.mutexes.size() ||
            thread.config.queueCount != thread.config.queue.size())
        {
            throw std::runtime_error
            ("count of elements in thread is not equal to actual: thread_" + std::to_string(thread.number));
        }
    }

    return _config;
}

void ConfigParser::getNameNumber(const IniSection& section, std::string& name, int& number)
{
    std::string firstPart;
    std::string secondPart;
    bool splitFlag = false;

    for (const auto& item : std::string(section))
    {
        if (item == parser::separatorInSection)
        {
            splitFlag = true;
        }
        else if (item == ' ')
        {}
        else if (splitFlag)
        {
            secondPart += item;
        }
        else
        {
            firstPart += item;
        }
    }

    try
    {
        number = std::stoi(secondPart);
        name = firstPart;
    }
    catch (...)
    {
        try
        {
            number = std::stoi(firstPart);
            name = secondPart;
        }
        catch (...)
        {
            throw std::runtime_error("incorrect format of section in line: " + std::to_string(section.getLineNum()));
        }
    }
}

void ConfigParser::keyExists(const IniSection& section, const std::string& key)
{
    if ( !_file.keyExists(section, key) )
    {
        throw std::runtime_error("missing key: " + std::string(key) + "\n" + "section line: " + section);
    }
}

size_t ConfigParser::getThreadIndex(const IniSection& section, int threadNum)
{
    auto thread = std::find_if(_config.begin(), _config.end(), [&threadNum](const threadConfig& thread){
        return thread.number == threadNum;
    });

    if (thread == _config.end())
    {
        throw std::runtime_error("incorrect section number in line: " + std::to_string(section.getLineNum()));
    }

    return std::distance(_config.begin(), thread);
}

void ConfigParser::readThread(const IniSection& section, int threadNum)
{
    threadConfig thread;

    try
    {
        keyExists(section, parser::mutexKey);
        keyExists(section, parser::queueKey);

        thread.config.mutexCount = _file.read<int>(section, parser::mutexKey, 0);
        thread.config.queueCount = _file.read<int>(section, parser::queueKey, 0);
        thread.number = threadNum;
    }
    catch(...)
    {
        throw;
    }

    _config.push_back( std::move(thread) );
}

void ConfigParser::readMutex(const IniSection& section, int threadNum)
{
    size_t threadIndex = getThreadIndex(section, threadNum);
    size_t count;
    mutexParams mParams;

    try
    {
        keyExists(section, parser::lockKey);

        count = _file.read<int>(section, parser::countKey, 1);
        mParams.lock = _file.read<bool>(section, parser::lockKey);
    }
    catch(...)
    {
        throw;
    }

    for (int i = 0; i < count; ++i)
    {
        _config[threadIndex].config.mutexes.push_back(mParams);
    }
}

void ConfigParser::readQueue(const IniSection& section, int threadNum)
{
    size_t threadIndex = getThreadIndex(section, threadNum);
    size_t count;
    queueParams qParams;

    try
    {
        keyExists(section, parser::sizeKey);
        keyExists(section, parser::emptyKey);
        keyExists(section, parser::fullKey);

        count = _file.read<int>(section, parser::countKey, 1);
        qParams.size = _file.read<int>(section, parser::sizeKey);
        qParams.isEmpty = _file.read<bool>(section, parser::emptyKey);
        qParams.isFull = _file.read<bool>(section, parser::fullKey);
    }
    catch (...)
    {
        throw;
    }

    for (int i = 0; i < count; ++i)
    {
        _config[threadIndex].config.queue.push_back(qParams);
    }
}

void ConfigParser::readTimer(const IniSection& section, int threadNum)
{

}

void ConfigParser::readSemaphore(const IniSection& section, int threadNum)
{

}
