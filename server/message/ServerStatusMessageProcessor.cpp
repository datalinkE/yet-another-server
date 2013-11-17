#include "ServerStatusMessageProcessor.h"

using namespace yas::proto;

ServerStatusMessageProcessor::ServerStatusMessageProcessor(MessageDispatcher * dispatcher)
    : mDispatcher(dispatcher)
{
}

std::string ServerStatusMessageProcessor::id() const
{
    return "ServerStatus";
}

ServerStatusAnswer ServerStatusMessageProcessor::doProcessing(const ServerStatusQuery & query)
{
    ServerStatusAnswer s;

    s.set_childcount(0);

    const MessageDispatcher::DispatcherMap & mdImpl = mDispatcher->registeredProcessors();

    for (auto it = mdImpl.begin(); it != mdImpl.end(); ++it)
    {
        s.add_messagehandlers(it->first);
    }

    return s;
}
