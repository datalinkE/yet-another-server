#include "StubMessageProcessor.h"


using namespace yas::proto;

StubMessageProcessor::StubMessageProcessor()
{
}


std::string StubMessageProcessor::id() const
{
    return "Stub";
}


StubAnswer StubMessageProcessor::doProcessing(const StubQuery & query)
{
    StubAnswer ans;
    ans.set_comment("does nothing yet");

	return ans;
}
