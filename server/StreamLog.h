#ifndef STREAMLOG_H
#define STREAMLOG_H

#include <ostream>
#include <streambuf>
#include <string>
#include <syslog.h>

/* inspired with
 * http://stackoverflow.com/questions/2638654/redirect-c-stdclog-to-syslog-on-unix */

enum LogPriority {
    kLogEmerg   = LOG_EMERG,   // system is unusable
    kLogAlert   = LOG_ALERT,   // action must be taken immediately
    kLogCrit    = LOG_CRIT,    // critical conditions
    kLogErr     = LOG_ERR,     // error conditions
    kLogWarning = LOG_WARNING, // warning conditions
    kLogNotice  = LOG_NOTICE,  // normal, but significant, condition
    kLogInfo    = LOG_INFO,    // informational message
    kLogDebug   = LOG_DEBUG    // debug-level message
};

std::ostream& operator<< (std::ostream& os, const LogPriority& log_priority);

class StreamLog : public std::basic_streambuf<char, std::char_traits<char> > {
public:
    explicit StreamLog(std::string ident, int facility);

protected:
    int sync();
    int overflow(int c);

private:
    friend std::ostream& operator<< (std::ostream& os, const LogPriority& log_priority);
    std::string _buffer;
    int _facility;
    int _priority;
    char _ident[50];
};

#endif // STREAMLOG_H
