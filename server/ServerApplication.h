#ifndef SERVERAPPLICATION_H
#define SERVERAPPLICATION_H

#include <string>
#include <memory>

#include "StreamLog.h"

class ServerApplicationImpl;

class ServerApplication
{
public:
    ServerApplication(unsigned short port, const std::string name = "yas-server");
    // all necessary system calls to set listening TCP stream socket
    // on the specified port should happen inside
    // it also redirects std::clog output to syslog() calls

    bool forkChildren(size_t children_count);
    // creates a specified number of child processes to handle incoming connections

    void stopChildren(size_t children_count = 0);
    // stops gracefully specified number of child processes
    // zero means all existing children

    size_t runningChildrenCount();
    // returns a number that is equal to a difference between all fork and stop calls (see above)
    // normally it should be the number of running child processes

    void loop();
    // parent or child loop inside

    void shutdown();
    // this call asks the application to finish all remaining work
    // and exit gracefully

private:
    std::shared_ptr<ServerApplicationImpl> _impl;
    // implementation is responsible for creating and terminating children

    std::shared_ptr<StreamLog> _log;
    // this StreamLog class redirects std::clog to syslog calls
};

#endif // SERVERAPPLICATION_H
