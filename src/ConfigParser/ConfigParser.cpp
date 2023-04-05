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
        if (thread.config.mutexCount != thread.config.mutexes.size()        ||
            thread.config.semaphoreCount != thread.config.semaphores.size() ||
            thread.config.queueCount != thread.config.queues.size()         ||
            thread.config.timerCount != thread.config.timers.size()         ||
            thread.config.eventCount != thread.config.events.size()         ||
            thread.config.condvarCount != thread.config.condvars.size()     ||
            thread.config.memoryCount != thread.config.memory.size()          )
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
        thread.config.mutexCount = _file.read<int>(section, parser::mutexKey, 0);
        thread.config.semaphoreCount = _file.read<int>(section, parser::semaphoreKey, 0);
        thread.config.queueCount = _file.read<int>(section, parser::queueKey, 0);
        thread.config.timerCount = _file.read<int>(section, parser::timerKey, 0);
        thread.config.eventCount = _file.read<int>(section, parser::eventKey, 0);
        thread.config.condvarCount = _file.read<int>(section, parser::condVarKey, 0);
        thread.config.memoryCount = _file.read<int>(section, parser::memoryKey, 0);
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

void ConfigParser::readSemaphore(const IniSection& section, int threadNum)
{
    size_t threadIndex = getThreadIndex(section, threadNum);
    size_t count;
    semaphoreParams sParams;

    try
    {
        keyExists(section, parser::stateKey);

        count = _file.read<int>(section, parser::countKey, 1);
        sParams.state = _file.read<bool>(section, parser::stateKey);
    }
    catch(...)
    {
        throw;
    }

    for (int i = 0; i < count; ++i)
    {
        _config[threadIndex].config.semaphores.push_back(sParams);
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
        _config[threadIndex].config.queues.push_back(qParams);
    }
}

void ConfigParser::readTimer(const IniSection& section, int threadNum)
{
    size_t threadIndex = getThreadIndex(section, threadNum);
    size_t count;
    timerParams tParams;

    try
    {
        keyExists(section, parser::timeoutKey);
        keyExists(section, parser::stateKey);

        count = _file.read<int>(section, parser::countKey, 1);
        tParams.timeout = _file.read<int>(section, parser::timeoutKey);
        tParams.state = _file.read<bool>(section, parser::stateKey);
    }
    catch(...)
    {
        throw;
    }

    for (int i = 0; i < count; ++i)
    {
        _config[threadIndex].config.timers.push_back(tParams);
    }
}

void ConfigParser::readEvent(const IniSection& section, int threadNum)
{
    size_t threadIndex = getThreadIndex(section, threadNum);
    size_t count;
    eventParams eParams;

    try
    {
        keyExists(section, parser::fStateKey);

        count = _file.read<int>(section, parser::countKey, 1);
        eParams.fState = _file.read<bool>(section, parser::fStateKey);
    }
    catch(...)
    {
        throw;
    }

    for (int i = 0; i < count; ++i)
    {
        _config[threadIndex].config.events.push_back(eParams);
    }
}

void ConfigParser::readCondvar(const IniSection& section, int threadNum)
{
    size_t threadIndex = getThreadIndex(section, threadNum);
    size_t count;
    condvarParams cParams;

    try
    {
        keyExists(section, parser::fStateKey);

        count = _file.read<int>(section, parser::countKey, 1);
    }
    catch(...)
    {
        throw;
    }

    for (int i = 0; i < count; ++i)
    {
        _config[threadIndex].config.condvars.push_back(cParams);
    }
}

void ConfigParser::readMemory(const IniSection& section, int threadNum)
{
    size_t threadIndex = getThreadIndex(section, threadNum);
    size_t count;
    memoryParams cParams;

    try
    {
        keyExists(section, parser::lSizeKey);
        keyExists(section, parser::bMapKey);
        keyExists(section, parser::timeoutKey);

        count = _file.read<int>(section, parser::countKey, 1);
        cParams.lSize = _file.read<bool>(section, parser::lSizeKey);
        cParams.bMap = _file.read<bool>(section, parser::bMapKey);
        cParams.timeout = _file.read<bool>(section, parser::timeoutKey);
    }
    catch(...)
    {
        throw;
    }

    for (int i = 0; i < count; ++i)
    {
        _config[threadIndex].config.memory.push_back(cParams);
    }
}

