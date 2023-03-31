#include "ConfigParser.h"

ConfigParser::ConfigParser(const std::string& path) : _file(path)
{}

std::vector<threadConfig> ConfigParser::getConfig()
{
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
                else if (std::find(parser::sections.begin(), parser::sections.end(), std::string(section)) == parser::sections.end())
                {
                    throw std::runtime_error("unknown section name in line: " + std::to_string(section.getLineNum()));
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

std::string ConfigParser::getKeyErrorMessage(const std::string& key, const IniSection& section)
{
    return "missing key: " + std::string(key) + "\n" + "section line: " + section;
}

void ConfigParser::readThread(const IniSection& section, int threadNum)
{
    threadConfig thread;

    if ( !_file.keyExists(section, parser::mutexKey) )
    {
        throw std::runtime_error( getKeyErrorMessage(parser::mutexKey, section) );
    }

    if ( !_file.keyExists(section, parser::queueKey) )
    {
        throw std::runtime_error( getKeyErrorMessage(parser::queueKey, section) );
    }

    thread.config.mutexCount = _file.read<int>(section, parser::mutexKey, 0);
    thread.config.queueCount = _file.read<int>(section, parser::queueKey, 0);
    thread.number = threadNum;

    _config.push_back( std::move(thread) );
}

void ConfigParser::readMutex(const IniSection& section, int threadNum)
{
    auto thread = std::find_if(_config.begin(), _config.end(), [&threadNum](const threadConfig& thread){
        return thread.number == threadNum;
    });

    if (thread == _config.end())
    {
        throw std::runtime_error("incorrect section number in line: " + std::to_string(section.getLineNum()));
    }

    if ( !_file.keyExists(section, parser::lockKey) )
    {
        throw std::runtime_error(getKeyErrorMessage(parser::lockKey, section) );
    }

    mutexParams mParams;
    size_t count = _file.read<int>(section, parser::countKey, 1);
    mParams.lock = _file.read<bool>(section, parser::lockKey);

    size_t threadIndex = std::distance(_config.begin(), thread);

    for (int i = 0; i < count; ++i)
    {
        _config[threadIndex].config.mutexes.push_back(mParams);
    }
}

void ConfigParser::readQueue(const IniSection& section, int threadNum)
{
    auto thread = std::find_if(_config.begin(), _config.end(), [&threadNum](const threadConfig& thread){
        return thread.number == threadNum;
    });

    if (thread == _config.end())
    {
        throw std::runtime_error("incorrect section number in line: " + std::to_string(section.getLineNum()));
    }

    if ( !_file.keyExists(section, parser::sizeKey) )
    {
        throw std::runtime_error(getKeyErrorMessage(parser::sizeKey, section) );
    }

    if ( !_file.keyExists(section, parser::emptyKey) )
    {
        throw std::runtime_error(getKeyErrorMessage(parser::emptyKey, section) );
    }

    if ( !_file.keyExists(section, parser::fullKey) )
    {
        throw std::runtime_error(getKeyErrorMessage(parser::fullKey, section) );
    }

    queueParams qParams;
    size_t count = _file.read<int>(section, parser::countKey, 1);
    qParams.size = _file.read<int>(section, parser::sizeKey);
    qParams.isEmpty = _file.read<bool>(section, parser::emptyKey);
    qParams.isFull = _file.read<bool>(section, parser::fullKey);

    size_t threadIndex = std::distance(_config.begin(), thread);

    for (int i = 0; i < count; ++i)
    {
        _config[threadIndex].config.queue.push_back(qParams);
    }
}

void ConfigParser::readTimer(const IniSection& section, int threadNum)
{
    auto thread = std::find_if(_config.begin(), _config.end(), [&threadNum](const threadConfig& thread){
        return thread.number == threadNum;
    });

    if (thread == _config.end())
    {
        throw std::runtime_error("incorrect section number in line: " + std::to_string(section.getLineNum()));
    }

    if ( !_file.keyExists(section, parser::timerKey) )
    {
        throw std::runtime_error(getKeyErrorMessage(parser::timerKey, section) );
    }

    mutexParams mParams;
    size_t count = _file.read<int>(section, parser::countKey, 1);
    mParams.lock = _file.read<bool>(section, parser::lockKey);

    size_t threadIndex = std::distance(_config.begin(), thread);

    for (int i = 0; i < count; ++i)
    {
        _config[threadIndex].config.mutexes.push_back(mParams);
    }
}

void ConfigParser::readSemaphore(const IniSection& section, int threadNum)
{

}
