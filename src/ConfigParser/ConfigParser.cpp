#include "ConfigParser.h"

ConfigParser::ConfigParser(const std::string& path) : _path(path)
{}

std::vector<threadConfig> ConfigParser::getConfig()
{
    std::string copyPath;

    try
    {
        copyPath = createFileCopy();
    }
    catch(...)
    {
        throw;
    }

    TRTIniFile file(copyPath, false);

    if ( !file.FileExists(_path) )
    {
        throw std::runtime_error("config file not exist at path - " + _path);
    }

    if ( !file.CanRead(_path) )
    {
        throw std::runtime_error("no permission to read config file - " + _path);
    }

    if ( !file.CanWrite(_path) )
    {
        throw std::runtime_error("no permission to write config file - " + _path);
    }

    _config.clear();

    for (auto& knownSection : parser::sections)
    {
        for (auto& section : file.GetSections())
        {
            std::string name;
            int number;
            bool readFlag = false;

            try
            {
                getNameNumber(section, name, number);

                if (name == knownSection)
                {
                    (_functionMap.find(name)->second)(this, std::ref(file), std::ref(section), number);
                    readFlag = true;
                }
            }
            catch (...)
            {
                throw;
            }

            // TRTIniFile can't delete last section in file and we leave it
            if (readFlag && file.GetSectionCount() > 1)
            {
                file.DeleteSection(section);
            }
        }
    }

    if (file.GetSectionCount() > 1)
    {
        std::string unknownSections;

        for (auto& section : file.GetSections())
        {
            unknownSections.append(section) += '\n';
        }

        throw std::runtime_error("unknown name in sections - " + unknownSections);
    }

    for (auto& thread : _config)
    {
        if (thread.config.mutexCount != thread.config.mutexes.size() ||
            thread.config.queueCount != thread.config.queue.size())
        {
            throw std::runtime_error
                    ("count of elements in thread is not equal to actual - thread_" + std::to_string(thread.number));
        }
    }

    return _config;
}

std::string ConfigParser::createFileCopy()
{
    std::string copyPath = _path;

    for (int i = copyPath.size() - 1; i > -1; --i)
    {
        if (copyPath[i] != '.' && std::ispunct(copyPath[i]))
        {
            break;
        }

        copyPath.pop_back();
    }

    copyPath += parser::fileCopyName;

    std::ifstream source(_path, std::ios::binary);
    std::ofstream destination(copyPath, std::ios::binary);

    if (source && destination)
    {
        destination << source.rdbuf();
    }
    else
    {
        throw std::runtime_error("can't copy file - " + _path);
    }

    return copyPath;
}

void ConfigParser::getNameNumber(const std::string& section, std::string& name, int& number)
{
    std::string firstPart;
    std::string secondPart;
    bool splitFlag = false;

    for (auto& item : section)
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
            throw std::runtime_error("incorrect format of section name - " + section);
        }
    }
}

std::string ConfigParser::getKeyErrorMessage(const std::string& key, const std::string& section)
{
    return "missing key - " + std::string(key) + "\n" + "section - " + section;
}

void ConfigParser::readThread(TRTIniFile& file, std::string& section, int number)
{
    threadConfig thread;

    if ( !file.KeyExists(section, parser::mutexKey) )
    {
        throw std::runtime_error( getKeyErrorMessage(parser::mutexKey, section) );
    }

    if ( !file.KeyExists(section, parser::queueKey) )
    {
        throw std::runtime_error( getKeyErrorMessage(parser::queueKey, section) );
    }

    thread.config.mutexCount = file.ReadInteger(section, parser::mutexKey, 0);
    thread.config.queueCount = file.ReadInteger(section, parser::queueKey, 0);
    thread.number = number;

    _config.push_back( std::move(thread) );
}

void ConfigParser::readMutex(TRTIniFile& file, std::string& section, int number)
{
    auto thread = std::find_if(_config.begin(), _config.end(), [&number](threadConfig& thread){
        return thread.number == number;
    });

    if (thread == _config.end())
    {
        throw std::runtime_error("incorrect number in section - " + section);
    }

    if ( !file.KeyExists(section, parser::lockKey) )
    {
        throw std::runtime_error(getKeyErrorMessage(parser::lockKey, section) );
    }

    mutexParams mParams;
    size_t count = file.ReadInteger(section, parser::countKey, 1);
    mParams.lock = file.ReadBool(section, parser::lockKey);

    size_t threadIndex = std::distance(_config.begin(), thread);

    for (int i = 0; i < count; ++i)
    {
        _config[threadIndex].config.mutexes.push_back(mParams);
    }
}

void ConfigParser::readQueue(TRTIniFile& file, std::string& section, int number)
{
    auto thread = std::find_if(_config.begin(), _config.end(), [&number](threadConfig& thread){
        return thread.number == number;
    });

    if (thread == _config.end())
    {
        throw std::runtime_error("incorrect number in section - " + section);
    }

    if ( !file.KeyExists(section, parser::sizeKey) )
    {
        throw std::runtime_error(getKeyErrorMessage(parser::sizeKey, section) );
    }

    if ( !file.KeyExists(section, parser::emptyKey) )
    {
        throw std::runtime_error(getKeyErrorMessage(parser::emptyKey, section) );
    }

    if ( !file.KeyExists(section, parser::fullKey) )
    {
        throw std::runtime_error(getKeyErrorMessage(parser::fullKey, section) );
    }

    queueParams qParams;
    size_t count = file.ReadInteger(section, parser::countKey, 1);
    qParams.size = file.ReadInteger(section, parser::sizeKey);
    qParams.isEmpty = file.ReadBool(section, parser::emptyKey);
    qParams.isFull = file.ReadBool(section, parser::fullKey);

    size_t threadIndex = std::distance(_config.begin(), thread);

    for (int i = 0; i < count; ++i)
    {
        _config[threadIndex].config.queue.push_back(qParams);
    }
}

void ConfigParser::readTimer(TRTIniFile& file, std::string& section, int number)
{
    auto thread = std::find_if(_config.begin(), _config.end(), [&number](threadConfig& thread){
        return thread.number == number;
    });

    if (thread == _config.end())
    {
        throw std::runtime_error("incorrect number in section - " + section);
    }

    if ( !file.KeyExists(section, parser::timerKey) )
    {
        throw std::runtime_error(getKeyErrorMessage(parser::timerKey, section) );
    }

    mutexParams mParams;
    size_t count = file.ReadInteger(section, parser::countKey, 1);
    mParams.lock = file.ReadBool(section, parser::lockKey);

    size_t threadIndex = std::distance(_config.begin(), thread);

    for (int i = 0; i < count; ++i)
    {
        _config[threadIndex].config.mutexes.push_back(mParams);
    }
}

void ConfigParser::readSemaphore(TRTIniFile& file, std::string& section, int number)
{

}
