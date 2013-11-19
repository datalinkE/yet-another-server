#include "ServerStatusMessageProcessor.h"

using namespace yas::proto;



ServerStatusMessageProcessor::ServerStatusMessageProcessor(ServerApplicationImpl* server)
    : _server(server)
{
}

std::string ServerStatusMessageProcessor::id() const
{
    return "ServerStatus";
}

ServerStatusAnswer ServerStatusMessageProcessor::doProcessing(const ServerStatusQuery & query)
{
    ServerStatusAnswer s;

    s.set_version(VERSION);

    const MessageDispatcher::DispatcherMap & mdImpl = _server->_dispatcher.registeredProcessors();

    for (auto it = mdImpl.begin(); it != mdImpl.end(); ++it)
    {
        s.add_messageprocessors(it->first);
    }

    return s;
}
