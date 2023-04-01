#ifndef MAINLOGIC_H
#define MAINLOGIC_H

#include "ConfigParser.h"
#include "WorkerThreads.h"

#include "log/TRTLog.h"
#include "log/TRTLogSender.h"

#include <iostream>
#include <algorithm>

namespace programInfo
{
    constexpr auto name = "cfp_simulator";
    constexpr auto version = "0.0.1";
}

namespace logging
{
    constexpr auto loggerName = "file_logger";
    constexpr auto loggerAppandFlag = false;
    constexpr auto logDirectory = "./";
}


class MainLogic
{
public:
    MainLogic(int argc, char* argv[]);

private:
    void runThreads();
    void startLogging();
    void parsingFileNameExtension();

    std::string _path;
    std::string _fileName;
    std::string _fileExtension;

    std::string _logPath;
};


#endif //MAINLOGIC_H
