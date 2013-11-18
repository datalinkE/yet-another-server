#ifndef SERVERAPPLICATIONIMPL_H
#define SERVERAPPLICATIONIMPL_H

#include <string>
#include <queue>
#include <memory>
#include "message/MessageDispatcher.h"

class ServerApplicationImpl
// actual internal implementation hides system calls complexity
// from front-end ServerApplication class
{
private:
    const std::string _name;
    const int _listen_count;
    const unsigned short _port;
    int _listenSocketId;
    bool _isListening;
    int _parrentPid;
    bool _shouldStop;
    std::queue<int> _childrenPids;
    // queue is for easy one-by-one creation and stopping in FIFO manner
    MessageDispatcher _dispatcher;


public:
// ServerApplicationImpl.cpp
    ServerApplicationImpl(unsigned short port, const std::string name = "yas-server");
    // the constructor makes all necessary system calls to set
    // listening TCP stream socket on the specified port

    bool forkChildren(size_t children_count);
    // creates a specified number of child processes to handle incoming connections

    void stopChildren(size_t children_count = 0);
    // stops no more then specified number of child processes sending them SIGTERM

    void parentProcessLoop();
    // this function is a main body of the parent process where
    // he waits his child to finish

// ServerApplicationImpl_child.cpp
    void childProcessLoop();
    // this function is a main body of any child process
    // and will go after fork() call


// ServerApplicationImpl_helpers.cpp
    void loop();
    // placeholder that should will be called from front-end class
    // internaly it calls parentProcessLoop() or childProcessLoop() respectively

    void shutdown();
    // this function sets _shouldStop variable which is false by default
    // then application do all remaining work and ends gracefully

    friend class ServerStatusMessageDispatcher;

private:
    bool createListeningSocket();
    bool setListeningSocketOptions();
    bool bindListeningSocket();
    bool runListening();
    // self-describing set of helper functions that should run
    // in order they are listet above

    bool isChild();
    // compares parrent pid with getpid() return value
};

#endif // SERVERAPPLICATIONIMPL_H
