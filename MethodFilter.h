#ifndef METHODFILTER_H
#define METHODFILTER_H

#include "PipelineStage.h"
#include <string>

class MethodFilter : public PipelineStage {
private:
    std::string targetMethod;
    PipelineStage* nextStage;

public:
    MethodFilter(std::string method, PipelineStage* next) : targetMethod(method), nextStage(next) {}

    void process(const LogRecord& record) override {
        if (record.method == targetMethod) {
            if (nextStage != nullptr) {
                nextStage->process(record);
            }
        }
    }
};

#endif
