#ifndef IPFILTER_H
#define IPFILTER_H

#include "PipelineStage.h"
#include <string>

class IpFilter : public PipelineStage {
private:
    std::string targetIp;
    PipelineStage* nextStage;

public:
    IpFilter(std::string ip, PipelineStage* next) : targetIp(ip), nextStage(next) {}

    void process(const LogRecord& record) override {
        // Eðer hedef IP eþleþiyorsa bir sonraki aþamaya ilet
        if (targetIp == "" || record.clientIp == targetIp) {
            if (nextStage != nullptr) {
                nextStage->process(record);
            }
        }
    }
};

#endif
