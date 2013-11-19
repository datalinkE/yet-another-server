#ifndef ECHOMESSAGEPROCESSOR_H
#define ECHOMESSAGEPROCESSOR_H

#include "Echo.pb.h"

#include "MessageProcessorVirtualTemplate.h"


class EchoMessageProcessor
        : public MessageProcessorVirtualTemplate<yas::proto::EchoQuery, yas::proto::EchoAnswer>
{
public:
	EchoMessageProcessor();

	virtual std::string id() const;


private:

    virtual yas::proto::EchoAnswer doProcessing(const yas::proto::EchoQuery & query);
};

#endif // ECHOMESSAGEPROCESSOR_H
