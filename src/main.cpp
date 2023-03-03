#include "ConfigParser.h"
#include "WorkerThreads.h"

int main(int argc, char *argv[])
{
    ConfigParser config("../config.txt");
    workerThreads( config.getConfig() );

    return 0;
}
