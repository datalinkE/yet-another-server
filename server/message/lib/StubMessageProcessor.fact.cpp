#include "../MessageProcessorFactoryMethod.h"
#include "../StubMessageProcessor.h"

// not sure if this is the right way
// but othervise i get name mangling and can't obtain my symbol
extern "C"
{
    SpMessageProcessorBase get_message_processor()
    {
        return std::make_shared<StubMessageProcessor>();
    }
}
