#ifndef MESSAGEPROCESSORBASE_H
#define MESSAGEPROCESSORBASE_H

#include <map>
#include <memory>
#include <stdexcept>

#include "Message.pb.h"

class MessageProcessingError: public std::runtime_error
{
public:
    MessageProcessingError(const std::string & e) :
        std::runtime_error(e)
    {
    }
};

class MessageProcessorBase
{
public:

    virtual ~MessageProcessorBase()
    {
    }

    virtual std::string id() const = 0;

    virtual yas::proto::Message process(const yas::proto::Message & query) = 0;
};

typedef std::shared_ptr<MessageProcessorBase> SpMessageProcessorBase;

#endif // MESSAGEPROCESSORBASE_H
