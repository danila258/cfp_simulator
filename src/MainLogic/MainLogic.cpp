#include "MainLogic.h"

MainLogic::MainLogic(int argc, char* argv[]) : _programName(argv[0])
{
    for (int i = 1; i < argc; ++i)
    {
        _pathsArr.emplace_back(argv[i]);
    }
}

MainLogic::MainLogic(const std::vector<UniversalString>& pathsArr, const UniversalString& programName, bool logFlag)
                     : _pathsArr(pathsArr), _programName(programName), _logFlag(logFlag)
{}

void MainLogic::run() try
{
    // find flag which disable logging and after delete it
    auto it = std::find(_pathsArr.begin(), _pathsArr.end(), flags::disableLog);

    if (it != _pathsArr.end())
    {
        _logFlag = false;
        _pathsArr.erase(it);
    }

    if (_pathsArr.size() == 1)
    {
        std::cerr << "missing file path" << '\n';
    }
    else if (_pathsArr.size() == 2)
    {
        _configPath = _pathsArr.back();
        runThreads();
    }
    else
    {
        for (const auto& path : _pathsArr)
        {
            // an explicit flush of std::cout is also necessary before a call to std::system
            std::cout.flush();

            std::string command = _programName;
            command.append(" ").append(path);

            if ( !_logFlag )
            {
                command += flags::disableLog;
            }

            std::system( command.c_str() );
        }
    }
}
catch (...)
{
    throw;
}

bool MainLogic::hasGUI()
{
    // find flag which disable GUI
    auto it = std::find(_pathsArr.begin(), _pathsArr.end(), flags::disableGui);

    if (it == _pathsArr.end())
    {
        return true;
    }

    return false;
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