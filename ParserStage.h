#ifndef PARSERSTAGE_H
#define PARSERSTAGE_H

#include "PipelineStage.h"
#include "LogRecord.h"
#include <string>
#include <sstream>

class ParserStage {
private:
    PipelineStage* nextStage;
    int errorCount;
    int validCount;

public:
    ParserStage(PipelineStage* next) : nextStage(next), errorCount(0), validCount(0) {}

    void parseAndProcess(const std::string& rawLine) {
        if (rawLine.empty()) {
            errorCount++;
            return;
        }

        std::stringstream ss(rawLine);
        std::string ip, dummy1, dummy2, dateBracket, request, statusStr, bytesStr;

        if (!(ss >> ip >> dummy1 >> dummy2 >> dateBracket)) {
            errorCount++;
            return;
        }

        std::string method = "", path = "", httpVersion = "";
        std::string quotePart;
        if (ss >> quotePart) {
            if (!quotePart.empty() && quotePart[0] == '"') {
                method = quotePart.substr(1);
            } else {
                method = quotePart;
            }
            ss >> path >> httpVersion;
        }

        if (!(ss >> statusStr >> bytesStr)) {
            errorCount++;
            return;
        }

        try {
            int status = std::stoi(statusStr);
            long bytes = (bytesStr == "-") ? 0 : std::stol(bytesStr);

            LogRecord record(dateBracket, ip, method, path, status, bytes, "Mozilla", rawLine);
            validCount++;

            if (nextStage != nullptr) {
                nextStage->process(record);
            }
        } catch (...) {
            errorCount++; // Hatalý satýrlar sayýlýr ve atlanýr[cite: 1]
        }
    }

    int getErrorCount() const { return errorCount; }
    int getValidCount() const { return validCount; }
};

#endif
