#include "MainLogic.h"

MainLogic::MainLogic(int argc, char* argv[])
{
    std::vector<std::string> pathsArr;
    std::string programName = argv[0];

    for (size_t i = 1; i < argc; ++i)
    {
        pathsArr.emplace_back(argv[i]);
    }

    // find flag which disable logging and after delete it
    auto it = std::find(pathsArr.begin(), pathsArr.end(), logging::disableLog);

    if (it != pathsArr.end())
    {
        _logFlag = false;
        pathsArr.erase(it);
    }

    if (pathsArr.size() == 1)
    {
        std::cout << "missing file path" << '\n';
    }
    else if (pathsArr.size() == 2)
    {
        _path = pathsArr.back();
        runThreads();
    }
    else
    {
        for (const auto& path : pathsArr)
        {
            // an explicit flush of std::cout is also necessary before a call to std::system
            std::cout.flush();

            std::string command = programName;
            command.append(" ").append(path);

            if ( !_logFlag )
            {
                command += logging::disableLog;
            }

            std::system( command.c_str() );
        }
    }
}

void MainLogic::runThreads()
{
    parsingFileNameExtension();

    if (_logFlag)
    {
        startLogging();
    }

    //ConfigParser parser(_path);
    //workerThreads(parser.getConfig());
}

void MainLogic::startLogging()
{
    TRTLog::Init(programInfo::name, programInfo::version, TRTLogSenderToFile(_fileName, logging::loggerAppendFlag, logging::logDirectory));
    rtlog(INFO) << _fileName + _fileExtension;
}

void MainLogic::parsingFileNameExtension()
{
    _logPath = _path;

    for (long long i = _logPath.size() - 1; i > -1; --i)
    {
        if (_logPath[i] != '.' && (_logPath[i] == '/' || _logPath[i] == '\\'))
        {
            break;
        }

        _fileName.push_back(_logPath[i]);
        _logPath.pop_back();
    }

    std::reverse(_fileName.begin(), _fileName.end());

    size_t dotPos = _fileName.find_last_of('.');

    if (dotPos != std::string::npos)
    {
        _fileExtension = _fileName.substr(dotPos);
        _fileName.erase(dotPos, _fileName.size() - dotPos);
    }
}