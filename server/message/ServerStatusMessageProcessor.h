#ifndef SERVERSTATUSMESSAGEPROCESSOR_H
#define SERVERSTATUSMESSAGEPROCESSOR_H

#include "MessageProcessorVirtualTemplate.h"
#include "ServerStatus.pb.h"
#include "../ServerApplicationImpl.h"

class ServerStatusMessageProcessor:
        public MessageProcessorVirtualTemplate<yas::proto::ServerStatusQuery, yas::proto::ServerStatusAnswer>
{
public:

    typedef yas::proto::ServerStatusQuery query_type;
    typedef yas::proto::ServerStatusAnswer answer_type;

    ServerStatusMessageProcessor(ServerApplicationImpl* server);

    virtual std::string id() const;

private:
    ServerApplicationImpl* _server;

    virtual answer_type doProcessing(const query_type & query);

    const int VERSION = 1;
};

#endif // SERVERSTATUSMESSAGEPROCESSOR_H
