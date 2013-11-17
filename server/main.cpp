#include <iostream>
#include <signal.h>
#include <unistd.h>

#include "ServerApplication.h"

#include "message/MessageProcessorBase.h"
#include "message/MessageDispatcher.h"

using namespace std;

const unsigned short PORT = 1972;

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

    server.forkChildren(2);

    server.loop();
}

