#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "ServerApplicationImpl.h"
#include "Message.pb.h"

using namespace std;

void ServerApplicationImpl::childProcessLoop()
{
    if(_isListening)
    {
        int newsock;
        vector<char> buffer;

        while (!_shouldStop)
        {
            newsock = accept(_listenSocketId, NULL ,NULL);

            if(newsock >= 0)
            {               
                clog << "client connected to child process " << getpid() << endl;

                size_t serial_size = 0;
                int nread = recv(newsock, &serial_size, sizeof(serial_size), 0);

                clog << "got message size, expecting " << serial_size << " bytes" << endl;

                if(buffer.size() < serial_size)
                {
                    clog << "resizing internal buffer" << endl;
                    buffer.resize(serial_size * 2);
                    // not sure if it is a good idea... waste of memory but not-so-often resizing
                }

                size_t bytes_recieved = 0;
                while(bytes_recieved < serial_size)
                {
                    nread = recv(newsock, buffer.data() + bytes_recieved, serial_size - bytes_recieved, 0);
                    if(nread > 0)
                    {
                        bytes_recieved += nread;
                        clog << "got " << nread << " bytes" << endl;
                    }
                    else
                        break;
                }

                try
                {
                    yas::proto::Message q;
                    q.ParseFromArray(buffer.data(), serial_size);


                    yas::proto::Message ans = _dispatcher.dispatch(q);

                    clog << "message dispatched" << endl;

                    string serial_ans = ans.SerializeAsString();
                    serial_size = serial_ans.length();

                    clog << "sending ans length " << serial_size << " back to client" << endl;
                    send(newsock, &serial_size, 4, 0);

                    size_t bytes_sent = 0;

                    while(bytes_sent < serial_size)
                    {
                        int nbytes = send(newsock, serial_ans.data() + bytes_sent, serial_size - bytes_sent, 0);
                        if(nbytes > 0)
                        {
                            bytes_sent += nbytes;
                            clog << "sending " << nbytes << " bytes back to client" << endl;
                        }
                        else
                            break;
                    }
                }
                catch(...)
                {
                    clog << "message can't be dispatched' " << getpid() << endl;
                }

                close(newsock);

                clog << "client disconnected from child process " << getpid() << endl;
            }
            else
            {
                clog << "child process " << getpid()
                       << " accept() fails with " << errno << endl;
            }
        }
    }
    clog << "child process " << getpid()
           << " closed " << endl;
}
