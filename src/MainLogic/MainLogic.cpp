#include "MainLogic.h"

#include <utility>

MainLogic::MainLogic(int argc, char* argv[]) : _programName(argv[0])
{
    for (int i = 1; i < argc; ++i)
    {
        _paths.emplace_back(argv[i]);
    }
}

void MainLogic::runFromConsole() try
{
    // find flag which disable logging and after delete it
    auto it = std::find(_paths.begin(), _paths.end(), flags::disableLog);

    if (it != _paths.end())
    {
        _logFlag = false;
        _paths.erase(it);
    }

    if (_paths.size() == 0)
    {
        std::cerr << "missing file path" << '\n';
    }
    else if (_paths.size() == 1)
    {
        _configPath = _paths.back();
        runThreads();
    }
    else
    {
        runProgramInstances();
    }
}
catch (...)
{
    throw;
}

// run a new instance of the program for each configuration file
void MainLogic::runProgramInstances()
{
    for (const auto& path : _paths)
    {
        // an explicit flush of std::cout is also necessary before a call to std::system
        std::cout.flush();

        std::string command = _programName;
        command.append(" ").append(path);
        command.append(" ").append(flags::disableGui);

        if ( !_logFlag )
        {
            command.append(" ").append(flags::disableLog);
        }

        std::system( command.c_str() );
    }
}

bool MainLogic::hasGUI()
{
    // find flag which disable GUI
    auto it = std::find(_paths.begin(), _paths.end(), flags::disableGui);

    if (it == _paths.end())
    {
        return true;
    }

    _paths.erase(it);

    return false;
}

void MainLogic::setPaths(const std::vector<UniversalString>& paths)
{
    _paths = paths;
}

std::vector<UniversalString> MainLogic::getPaths()
{
    return _paths;
}

void MainLogic::runThreads() try
{
    parsingFileNameExtension();

    if (_logFlag)
    {
        startLogging();
    }

    ConfigParser parser(_configPath);

    WorkerThreads worker(parser.getThreads(), parser.getActions());
    worker.run();
}
catch (...)
{
    throw;
}

void MainLogic::startLogging()
{
    TRTLog::Init(programInfo::name, programInfo::version,
                 TRTLogSenderToFile(_configName, logging::loggerAppendFlag, logging::logDirectory));

    rtlog(INFO) << _configName + _configExtension;
}

void MainLogic::parsingFileNameExtension()
{
    _logPath = _configPath;

    for (long long i = _logPath.size() - 1; i > -1; --i)
    {
        if (_logPath[i] != '.' && (_logPath[i] == '/' || _logPath[i] == '\\'))
        {
            break;
        }

        _configName.push_back(_logPath[i]);
        _logPath.pop_back();
    }

    std::reverse(_configName.begin(), _configName.end());

    size_t dotPos = _configName.find_last_of('.');

    if (dotPos != std::string::npos)
    {
        _configExtension = _configName.substr(dotPos);
        _configName.erase(dotPos, _configName.size() - dotPos);
    }
}