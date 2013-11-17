#include "StreamLog.h"
#include <cstring>

using namespace std;

StreamLog::StreamLog(::string ident, int facility)
{
    _facility = facility;
    _priority = LOG_DEBUG;
    ::strncpy(_ident, ident.c_str(), sizeof(_ident));
    _ident[sizeof(_ident)-1] = '\0';

    openlog(_ident, LOG_PID, _facility);
}

int StreamLog::sync()
{
    if (_buffer.length())
    {
        syslog(_priority, _buffer.c_str());
        _buffer.erase();
        _priority = LOG_DEBUG; // default to debug for each message
    }
    return 0;
}

int StreamLog::overflow(int c)
{
    if (c != EOF)
    {
        _buffer += static_cast<char>(c);
    }
    else
    {
        sync();
    }
    return c;
}

::ostream& operator<< (::ostream& os, const LogPriority& log_priority)
{
    static_cast<StreamLog *>(os.rdbuf())->_priority = (int)log_priority;
    return os;
}
