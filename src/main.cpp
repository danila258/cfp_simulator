#include "ConfigParser.h"
#include "WorkerThreads.h"
#include "spdlog/spdlog.h"

int main(int argc, char* argv[])
{
    ConfigParser config("./config.txt");
    config.printConfig();

    workerThreads( config.getConfig() );

    spdlog::info("Test log");

    return 0;
}
