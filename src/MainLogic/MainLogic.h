#ifndef MAINLOGIC_H
#define MAINLOGIC_H

#include "ConfigParser.h"
#include "WorkerThreads.h"
#include "UniversalString.h"

#include "log/TRTLog.h"
#include "log/TRTLogSender.h"

#include <iostream>
#include <algorithm>
#include <vector>


namespace programInfo
{
    constexpr auto name = "cfp_simulator";
    constexpr auto version = "0.0.1";
}

namespace flags
{
    constexpr auto disableGui = "GUI_OFF";
    constexpr auto disableLog = "LOG_OFF";
}

namespace logging
{
    constexpr auto loggerAppendFlag = false;
    constexpr auto logDirectory = "./";
}


class MainLogic
{
public:
    MainLogic(int argc, char* argv[]);

    void runFromConsole();
    void runProgramInstances();

    bool hasGUI();
    void setLoggingState(bool state);

    void setPaths(const std::vector<UniversalString>& paths);
    std::vector<UniversalString> getPaths();

private:
    void runThreads();
    void startLogging();
    void parsingFileNameExtension();

    std::vector<UniversalString> _paths;
    std::string _programName;

    std::string _configPath;
    std::string _configName;
    std::string _configExtension;

    std::string _logPath;
    bool _logFlag = true;
};


#endif //MAINLOGIC_H
