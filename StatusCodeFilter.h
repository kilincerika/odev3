#ifndef STATUSCODEFILTER_H
#define STATUSCODEFILTER_H

#include "PipelineStage.h"

class StatusCodeFilter : public PipelineStage {
private:
    int targetStatus;
    PipelineStage* nextStage;

public:
    StatusCodeFilter(int status, PipelineStage* next) : targetStatus(status), nextStage(next) {}

    void process(const LogRecord& record) override {
        if (record.status == targetStatus) {
            if (nextStage != nullptr) {
                nextStage->process(record);
            }
        }
    }
};

#endif
