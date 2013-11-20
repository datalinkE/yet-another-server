#ifndef ECHOMESSAGEPROCESSOR_H
#define ECHOMESSAGEPROCESSOR_H

#include "Stub.pb.h"

#include "MessageProcessorVirtualTemplate.h"


class StubMessageProcessor
        : public MessageProcessorVirtualTemplate<yas::proto::StubQuery, yas::proto::StubAnswer>
{
public:
    StubMessageProcessor();

	virtual std::string id() const;


private:

    virtual yas::proto::StubAnswer doProcessing(const yas::proto::StubQuery & query);
};

#endif // ECHOMESSAGEPROCESSOR_H
