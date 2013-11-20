#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <google/protobuf/stubs/common.h>
#include <stdexcept>

#include "ServerApplicationImpl.h"
#include "Message.pb.h"

using namespace std;

const size_t MAX_SERIAL_SIZE_INPUT = 0x1000;
// not a very good way... but better then just allow anything

void ServerApplicationImpl::childProcessLoop()
{
    // socket has timeout set, so recv/send calls will not block forever
    // see ..._helpres/setListeningSocketOptions()

    google::protobuf::LogSilencer silence; // comment this if you want protobuf debug messages at stderr

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

                try
                {
                    size_t serial_size = 0;
                    size_t serial_size_net = 0;

                    int nread = recv(newsock, &serial_size_net, sizeof(serial_size_net), 0);
                    serial_size = ntohl(serial_size_net);

                    clog << "got message size, expecting " << serial_size << " bytes" << endl;
                    if(serial_size > MAX_SERIAL_SIZE_INPUT)
                    {
                        throw std::runtime_error("client tries to send too much data");
                    }

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


                    yas::proto::Message q;
                    if ( !q.ParseFromArray(buffer.data(), serial_size))
                    {
                        throw std::runtime_error("bad request, can't parse");
                    }

                    yas::proto::Message ans = _dispatcher.dispatch(q); // can throw too

                    clog << "message dispatched" << endl;

                    string serial_ans = ans.SerializeAsString();
                    serial_size = serial_ans.length();

                    clog << "sending ans length " << serial_size << " back to client" << endl;
                    serial_size_net = htonl(serial_size);
                    send(newsock, &serial_size_net, sizeof(size_t), 0);

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
                catch(std::runtime_error e)
                {
                    clog << e.what() << endl;
                    clog << "message can't be dispatched' " << getpid() << endl;
                }

                close(newsock);

                clog << "client disconnected from child process " << getpid() << endl;
            }
            else
            {
                if(errno == EAGAIN || errno == EWOULDBLOCK)
                    continue; // cause we set timeout walue to socket

                clog << "child process " << getpid()
                       << " accept() fails with " << errno << endl;
            }
        }
    }
    clog << "child process " << getpid()
           << " closed " << endl;
}
