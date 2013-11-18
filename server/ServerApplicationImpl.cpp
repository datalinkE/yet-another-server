#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "ServerApplicationImpl.h"
#include "message/ServerStatusMessageProcessor.h"

using namespace std;

ServerApplicationImpl::ServerApplicationImpl(unsigned short port, const string name) :
    _name(name),
    _listen_count(5),       // suggested in Apress "Definitive guide..." book as default value for listening queue
    _listenSocketId(-1),    // valid descriptors are non-negative
    _port(port),
    _isListening(false),
    _shouldStop(false)
// the constructor makes all necessary system calls to set
// listening TCP stream socket on the specified port
{
    clog  << endl << "server started" << endl;

    _parrentPid = getpid();

    _dispatcher.addProcessor( make_shared<ServerStatusMessageProcessor> (&_dispatcher));

    _isListening =
        createListeningSocket() &&
        setListeningSocketOptions() &&
        bindListeningSocket() &&
        runListening();
    if(_isListening)
        clog << "is listening on port " << _port << endl;
}


bool ServerApplicationImpl::forkChildren(size_t children_count)
// creates a specified number of child processes to handle incoming connections
{
    if(_isListening)
    {
        clog << "creating " << children_count << " worker processes" << endl;

        for (size_t i = 0; i < children_count; i++)
        {
            int pid = fork();
            if (pid == 0)
            {
                // here we go into child process
                return true;
            }
            else if (pid > 0)
            {
                _childrenPids.push(pid);
                clog << "child process with " << pid <<  " created" << endl;
            }
            else
            {
                clog << "child process creation error " << errno << endl;
            }
        }
        return true;
    }
    else
        return false;
}


void ServerApplicationImpl::parentProcessLoop()
{
    if(_isListening)
    {
        while(!_shouldStop)
        {
            sleep(1);
        }

        stopChildren();
        clog << "waiting for child processes termination" << endl;
        wait(NULL); // waiting all childs to finish
    }

    clog << "parent closed" << endl;
    clog << "server stopped" << endl;
}

void ServerApplicationImpl::stopChildren(size_t children_count)
{
    size_t i = 0;
    size_t count = (children_count == 0 ? _childrenPids.size() :
                                          ::min(_childrenPids.size(), children_count));

    while(i < count)
    {
        int pid = _childrenPids.front();
        clog << "sending SIGTERM to child " << pid << endl;
        kill(pid, SIGTERM);
        _childrenPids.pop();
        i++;
    }
}
