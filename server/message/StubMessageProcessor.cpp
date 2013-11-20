#include <ctime>
#include <string>
#include <sstream>
#include "StubMessageProcessor.h"


using namespace yas::proto;
using namespace std;


string get_primes(uint32_t up_to = 10000) {
    clock_t start, end;
    double runTime;
    start = clock();
    uint32_t i, num = 1, primes = 0;

    while (num <= up_to) {
        i = 2;
        while (i <= num) {
            if(num % i == 0)
                break;
            i++;
        }
        if (i == num)
            primes++;

        num++;
    }

    end = clock();
    runTime = (end - start) / (double) CLOCKS_PER_SEC;
    ostringstream result;
    result << "This machine calculated all " << primes
           << " prime numbers under " << up_to
           << " in " << runTime << " seconds";
    return result.str();
}


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

    query.has_seed() ?
            ans.set_comment(get_primes(query.seed()))
          : ans.set_comment(get_primes());

	return ans;
}
