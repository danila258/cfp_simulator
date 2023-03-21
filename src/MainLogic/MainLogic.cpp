#include "MainLogic.h"

void MainLogic::startProgram(int argc, char* argv[])
{
    if (argc == 1)
    {
        std::cout << "missing file path" << '\n';
    }
    else if (argc == 2)
    {
        _path = argv[1];
        runThreads();
    }
    else
    {
        for (int i = 1; i < argc; ++i)
        {
            // an explicit flush of std::cout is also necessary before a call to std::system
            std::cout.flush();

            std::string command;
            command.append(argv[0]).append(" ").append(argv[i]);

            std::system( command.c_str() );
        }
    }
}

void MainLogic::runThreads()
{
    ConfigParser parser(_path);

    startLogging();
    workerThreads(parser.getConfig(), _logger);
}

void MainLogic::startLogging()
{
    std::string logPath = _path;
    std::string fileName;

    for (int i = logPath.size() - 1; i > -1; --i)
    {
        if (logPath[i] != '.' && std::ispunct(logPath[i]))
        {
            break;
        }

        fileName.push_back(logPath[i]);
        logPath.pop_back();
    }

    std::reverse(fileName.begin(), fileName.end());
    std::string fileNameCopy = fileName;

    size_t dotPos = fileName.find_last_of('.');

    if (dotPos != fileName.npos)
    {
        fileName.erase(dotPos, fileName.size() - dotPos);
    }

    _logger = spdlog::basic_logger_st("basic_logger", logPath + logging::logName + fileName + logging::logFormat);
    _logger->set_pattern("[%l] %v");
    _logger->info(fileNameCopy);
}
