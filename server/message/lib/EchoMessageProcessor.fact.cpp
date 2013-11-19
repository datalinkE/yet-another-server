#include "../MessageProcessorFactoryMethod.h"
#include "../EchoMessageProcessor.h"

SpMessageProcessorBase get_message_processor()
{
    return std::make_shared<EchoMessageProcessor>();
}
