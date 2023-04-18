#include "MainLogic.h"

MainLogic::MainLogic(int argc, char* argv[])
{
    if (argc == 1)
    {
        std::cout << "missing file path" << '\n';
    }
    else if (argc == 2 || argc == 3 && (strcmp(argv[1], logging::disableLog) == 0 || strcmp(argv[2], logging::disableLog) == 0))
    {
        if (argc == 3)
        {
            _logFlag = false;
            _path = (strcmp(argv[1], logging::disableLog))? argv[1] : argv[2];
        }
        else
        {
            _path = argv[1];
        }

        runThreads();
    }
    else
    {
        for (int i = 1; i < argc; ++i)
        {
            if (strcmp(argv[i], "log=off") == 0)
            {
                _logFlag = false;
                break;
            }
        }

        for (int i = 1; i < argc; ++i)
        {
            // an explicit flush of std::cout is also necessary before a call to std::system
            std::cout.flush();

            std::string command;

            if (_logFlag)
            {
                command.append(argv[0]).append(" ").append(argv[i]);
            }
            else
            {
                if (strcmp(argv[i], logging::disableLog))
                {
                    command.append(argv[0]).append(" ").append(argv[i]).append(" ").append(logging::disableLog);
                }
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

    ConfigParser parser(_path);
    workerThreads(parser.getConfig());
}

void MainLogic::startLogging()
{
    TRTLog::Init(programInfo::name, programInfo::version, TRTLogSenderToFile(_fileName, logging::loggerAppendFlag, logging::logDirectory));
    rtlog(INFO) << _fileName + _fileExtension;
}

void MainLogic::parsingFileNameExtension()
{
    _logPath = _path;

    for (int i = _logPath.size() - 1; i > -1; --i)
    {
        if (_logPath[i] != '.' && std::ispunct(_logPath[i]))
        {
            break;
        }

        _fileName.push_back(_logPath[i]);
        _logPath.pop_back();
    }

    std::reverse(_fileName.begin(), _fileName.end());

    size_t dotPos = _fileName.find_last_of('.');
    _fileExtension = _fileName.substr(dotPos);
    _fileName.erase(dotPos, _fileName.size() - dotPos);
}
