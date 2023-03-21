#ifndef MAINLOGIC_H
#define MAINLOGIC_H

#include "ConfigParser.h"
#include "WorkerThreads.h"

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/logger.h"

#include <iostream>
#include <algorithm>


namespace logging
{
    constexpr auto logName = "log_";
    constexpr auto logFormat = ".txt";
}


class MainLogic
{
public:
    void startProgram(int argc, char* argv[]);

private:
    void runThreads();
    void startLogging();

    std::string _path;
    std::shared_ptr<spdlog::logger> _logger;
};

#endif //MAINLOGIC_H
