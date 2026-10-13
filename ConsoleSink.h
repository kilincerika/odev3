#ifndef CONSOLESINK_H
#define CONSOLESINK_H

#include "PipelineStage.h"
#include <iostream>
#include <iomanip>

class ConsoleSink : public PipelineStage {
public:
    void process(const LogRecord& record) override {
        std::cout << "[YAZDIRILDI] IP: " << std::setw(15) << record.clientIp 
                  << " | Metot: " << std::setw(6) << record.method 
                  << " | Yol: " << std::setw(14) << record.path 
                  << " | Durum: " << record.status 
                  << " | Boyut: " << record.bytes << " bayt" << std::endl;
    }
};

#endif
