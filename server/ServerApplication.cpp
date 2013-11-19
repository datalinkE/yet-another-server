#include <iostream>
#include <memory>

#include "ServerApplication.h"
#include "ServerApplicationImpl.h"

using namespace std;

ServerApplication::ServerApplication(unsigned short port, const ::string name)
{
    _log = make_shared<StreamLog>( name, LOG_LOCAL0 );
    std::clog.rdbuf(_log.get());

    _impl = make_shared<ServerApplicationImpl>(port, name);
}

bool ServerApplication::forkChildren(size_t children_count)
{
    return _impl->forkChildren(children_count);
}

void ServerApplication::stopChildren(size_t children_count)
{
    _impl->stopChildren(children_count);
}

size_t ServerApplication::runningChildrenCount()
{
    return _impl->runningChildrenCount();
}

void ServerApplication::shutdown()
{
    _impl->shutdown();
}

void ServerApplication::loop()
{
    _impl->loop();
}


