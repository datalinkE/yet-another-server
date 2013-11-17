#ifndef MESSAGEDISPATCHER_H
#define MESSAGEDISPATCHER_H

#include <map>
#include <string>
#include "Message.pb.h"

#include "MessageProcessorBase.h"

class MessageDispatcher
{
public:
    MessageDispatcher();

    void addProcessor(SpMessageProcessorBase processor);

    yas::proto::Message dispatch(const yas::proto::Message & query);

    typedef std::map<std::string, SpMessageProcessorBase> DispatcherMap;

    const DispatcherMap & registeredProcessors() const;

private:
    DispatcherMap _processors;
};

#endif // MESSAGEDISPATCHER_H
