#include "MainLogic.h"

void MainLogic::startProgram(int argc, char* argv[])
{
    if (argc == 1)
    {
        std::cout << "missing file path" << '\n';
    }
    else if (argc == 2)
    {
        runThreads(argv[1]);
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

void MainLogic::runThreads(const std::string& path)
{
    ConfigParser config(path);
    config.printConfig();
    workerThreads( config.getConfig() );
}
