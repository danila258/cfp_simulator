#include "MainLogic.h"

MainLogic::MainLogic(int argc, char* argv[])
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
    workerThreads(parser.getConfig());
}

void MainLogic::startLogging()
{
    parsingFileNameExtension();

    TRTLog::Init(programInfo::name, programInfo::version, TRTLogSenderToFile(_fileName, logging::loggerAppandFlag, logging::logDirectory));
    //rtlog.addFileLog("module A", TRTLogSenderToFile(_fileName + logging::logFormat, logging::loggerAppandFlag, logging::logDirectory));
    rtlog(INFO) << _fileName + _fileExtension;
}

void MainLogic::parsingFileNameExtension()
{
    std::string logPath = _path;

    for (int i = logPath.size() - 1; i > -1; --i)
    {
        if (logPath[i] != '.' && std::ispunct(logPath[i]))
        {
            break;
        }

        _fileName.push_back(logPath[i]);
        logPath.pop_back();
    }

    std::reverse(_fileName.begin(), _fileName.end());

    size_t dotPos = _fileName.find_last_of('.');
    _fileExtension = _fileName.substr(dotPos);
    _fileName.erase(dotPos, _fileName.size() - dotPos);

    //clear previous log file
//    std::ofstream fileClearStream;
//    fileClearStream.open(logPath + _fileName + logging::logFormat, std::ofstream::out | std::ofstream::trunc);

//    if ( !fileClearStream )
//    {
//        throw std::runtime_error("can't clear log file");
//    }

//    fileClearStream.close();
}
