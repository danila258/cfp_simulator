#ifndef MAINLOGIC_H
#define MAINLOGIC_H

#include "ConfigParser.h"
#include "WorkerThreads.h"

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/logger.h"

#include "log/TRTLog.h"
#include "log/TRTLogSender.h"

#include <iostream>
#include <algorithm>


namespace logging
{
    constexpr auto logFormat = ".log";
    constexpr auto logDirectory = "./";
}


class MainLogic
{
public:
    void startProgram(int argc, char* argv[]);

private:
    void runThreads();
    void startLogging();
    void parsingFileNameExtension();

    std::string _path;
    std::string _fileName;
    std::string _fileExtension;

    std::shared_ptr<spdlog::logger> _logger;
    std::string _logPath;

    TRTLogSenderToFile _logger2();
};


#endif //MAINLOGIC_H
