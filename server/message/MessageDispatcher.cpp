#include "MessageDispatcher.h"

using namespace yas::proto;

MessageDispatcher::MessageDispatcher()
{
}

void MessageDispatcher::addProcessor(SpMessageProcessorBase processor)
{
    _processors[processor->id()] = processor;
}

Message MessageDispatcher::dispatch(const Message & query)
{
    DispatcherMap::iterator it = _processors.find(query.id());

    if (it == _processors.end())
    {
        throw std::runtime_error("unknown message id: " + query.id());
    }

    return it->second->process(query);
}

const MessageDispatcher::DispatcherMap & MessageDispatcher::registeredProcessors() const
{
    return _processors;
}
