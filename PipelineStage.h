#ifndef PIPELINESTAGE_H
#define PIPELINESTAGE_H

#include "LogRecord.h"

class PipelineStage {
public:
    virtual ~PipelineStage() {}
    virtual void process(const LogRecord& record) = 0;
};

#endif
