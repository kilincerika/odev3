#ifndef LOGRECORD_H
#define LOGRECORD_H

#include <string>
#include <map>

struct LogRecord {
    std::string timestamp;
    std::string clientIp;
    std::string method;
    std::string path;
    int status;
    long bytes;
    std::string userAgent;
    std::map<std::string, std::string> attributes; // Gelecek haftalardaki geniþlemeler için[cite: 1]
    std::string raw;

    LogRecord(std::string ts, std::string ip, std::string m, std::string p, int st, long b, std::string ua, std::string r)
        : timestamp(ts), clientIp(ip), method(m), path(p), status(st), bytes(b), userAgent(ua), raw(r) {}
};

#endif
