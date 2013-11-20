#include <iostream>
#include <thread>
#include <signal.h>
#include <unistd.h>

#include "ServerApplication.h"

#include "message/MessageProcessorBase.h"
#include "message/MessageDispatcher.h"

using namespace std;

const unsigned short PORT = 1972;
const size_t DEFAULT_NUM_CPU = 2;

ServerApplication* appPointer = nullptr;

struct sigaction action;

void term(int signum)
{
    if(appPointer)
        appPointer->shutdown();
}


int main(int argc, char *argv[])
{
    action.sa_handler = term;
    sigaction(SIGTERM, &action, NULL);

    ServerApplication server(PORT);

    appPointer = &server;

    // dunno which way is the best
    size_t num_cpu = std::thread::hardware_concurrency();
    if(num_cpu == 0) // may return 0 when not able to detect
    {
        num_cpu = sysconf( _SC_NPROCESSORS_ONLN );
    }

    if(num_cpu < 2 || num_cpu > 16) num_cpu = DEFAULT_NUM_CPU; // rollback to default if values are strange

    server.forkChildren(num_cpu);

    server.loop();
}

