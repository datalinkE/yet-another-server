#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "ServerApplicationImpl.h"

using namespace std;

void ServerApplicationImpl::childProcessLoop()
{
    if(_isListening)
    {
        int newsock;
        char buffer[25];
        int nread;

        while (!_shouldStop)
        {
            newsock = accept(_listenSocketId, NULL ,NULL);

            if(newsock >= 0)
            {
                nread = recv(newsock, buffer, 25, 0);

                ::clog << "client connected to child process" << getpid() << ::endl;

                buffer[nread] = '\0';
                ::clog << buffer << ::endl;
                send(newsock, buffer, nread, 0);
                close(newsock);

                ::clog << "client disconnected from child process" << getpid() << ::endl;
            }
            else
            {
                ::clog << "child process " << getpid()
                       << " accept() fails with " << errno << ::endl;
            }
        }
    }
    ::clog << "child process " << getpid()
           << " closed " << ::endl;
}
