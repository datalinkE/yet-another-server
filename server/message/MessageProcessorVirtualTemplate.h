#ifndef MESSAGEPROCESSORVIRTUALTEMPLATE_H
#define MESSAGEPROCESSORVIRTUALTEMPLATE_H

#include "MessageProcessorBase.h"

template <typename ProtoQueryT, typename ProtoAnswerT>
class MessageProcessorVirtualTemplate: public MessageProcessorBase
{
public:
    virtual yas::proto::Message process(const yas::proto::Message & query)
    {
        ProtoQueryT underlyingQuery;
        if (!underlyingQuery.ParseFromString(query.data()))
        {
            throw MessageProcessingError("Failed to parse query: " +
                query.ShortDebugString());
        }

        ProtoAnswerT underlyingAnswer = doProcessing(underlyingQuery);

        yas::proto::Message a;
        a.set_id(query.id());

        if (!underlyingAnswer.SerializeToString(a.mutable_data()))
        {
            throw MessageProcessingError("Failed to prepare answer: " +
                underlyingAnswer.ShortDebugString());
        }
        return a;
    }

private:
    virtual ProtoAnswerT doProcessing(const ProtoQueryT & query) = 0;
};

#endif // MESSAGEPROCESSORVIRTUALTEMPLATE_H
