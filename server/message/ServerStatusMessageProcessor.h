#ifndef SERVERSTATUSMESSAGEPROCESSOR_H
#define SERVERSTATUSMESSAGEPROCESSOR_H

#include "MessageProcessorVirtualTemplate.h"
#include "MessageDispatcher.h"
#include "ServerStatus.pb.h"

class ServerStatusMessageProcessor:
        public MessageProcessorVirtualTemplate<yas::proto::ServerStatusQuery, yas::proto::ServerStatusAnswer>
{
public:

    typedef yas::proto::ServerStatusQuery query_type;
    typedef yas::proto::ServerStatusAnswer answer_type;

    ServerStatusMessageProcessor(MessageDispatcher * dispatcher);

    virtual std::string id() const;

private:
    MessageDispatcher * mDispatcher;

    virtual answer_type doProcessing(const query_type & query);
};

#endif // SERVERSTATUSMESSAGEPROCESSOR_H
