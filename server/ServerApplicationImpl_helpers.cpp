#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <dlfcn.h>

#include <string>

#include "ServerApplicationImpl.h"

#include "message/MessageProcessorFactoryMethod.h"

using namespace std;

bool ServerApplicationImpl::isChild()
{
    return _parrentPid != getpid();
}

size_t ServerApplicationImpl::runningChildrenCount()
{
    return _childrenPids.size();
}

void ServerApplicationImpl::shutdown()
{
    clog
           << ( isChild() ? "child" : "parent")
           << " termination requested" << endl;
    _shouldStop = true;
}

void ServerApplicationImpl::loop()
{
    if(isChild())
        childProcessLoop();
    else
        parentProcessLoop();
}

bool ServerApplicationImpl::createListeningSocket()
{
    _listenSocketId = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(_listenSocketId < 0)
    {
        clog << "socket() fails with " << errno << endl;
        return false;
    }

    return true;
}

bool ServerApplicationImpl::setListeningSocketOptions()
{
    int call_result;

    int val = 1;
    call_result = setsockopt(_listenSocketId, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

    if (call_result < 0)
    {
        clog << "setsockopt() fails with " << call_result << endl;
        return false;
    }
    return true;
}

bool ServerApplicationImpl::bindListeningSocket()
{
    int call_result;

    struct sockaddr_in sAddr;

    sAddr.sin_family = AF_INET;
    sAddr.sin_port = htons(_port);
    sAddr.sin_addr.s_addr = INADDR_ANY;

    call_result = bind(_listenSocketId, (struct sockaddr *) &sAddr, sizeof(sAddr));
    if (call_result < 0)
    {
        clog << "bind() fails with " << call_result << endl;
        return false;
    }
    return true;
}

bool ServerApplicationImpl::runListening()
{
    int call_result;

    call_result = listen(_listenSocketId, _listen_count);
    if (call_result < 0)
    {
        clog << "listen() fails with" << call_result << endl;
        return false;
    }
    return true;
}


void ServerApplicationImpl::loadMessageProcessorFromLibrary(std::string name)
{
    void* handle = dlopen(name.c_str(), RTLD_LAZY);

    if(handle == NULL)
    {
        clog << dlerror() << endl;
        return;
    }

    _loadedLibs.push_back(handle);
    clog << name << " loaded" << endl;


    MessageProcessorFactoryMethod fact =
            reinterpret_cast<MessageProcessorFactoryMethod>(dlsym(handle, "get_message_processor"));
    // ugly, but sustem call returns void* function pointer

    if(fact)
    {
        SpMessageProcessorBase mp = fact();
        _dispatcher.addProcessor(mp);
    }
    else
    {
        clog << dlerror() << endl;
        clog << "error loading factory method" << endl;
    }
}
