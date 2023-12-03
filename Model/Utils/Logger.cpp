//
// Created by vlad on 10/11/23.
//

#include "Logger.h"

#include <utility>

std::vector<std::string> Logger::logs;
std::string Logger::filename;

Logger::Logger(std::string _filename) {
    filename = std::move(_filename);
};

void Logger::log(double currentTime, const std::string& name, const std::string& event, std::optional<int> processing,
                 std::optional<int> processed, std::optional<int> blocked, std::optional<int> queueSize,
                 std::optional<int> attempt) {
    std::string logStr;
    logStr += std::to_string(currentTime) + ",";
    logStr += (!name.empty() ? name : "") + ",";
    logStr += (!event.empty() ? event : "") + ",";
    logStr += (processing.has_value() ? std::to_string(processing.value()) : "") + ",";
    logStr += (processed.has_value() ? std::to_string(processed.value()) : "") + ",";
    logStr += (blocked.has_value() ? std::to_string(blocked.value()) : "") + ",";
    logStr += (queueSize.has_value() ? std::to_string(queueSize.value()) : "") + ",";
    logStr += (attempt.has_value() ? std::to_string(attempt.value()) : "") + "\n";
    logs.push_back(logStr);
}

void Logger::saveLogFile() {
    std::ofstream myfile(filename);
    myfile << "Current Time,Name,Event,Busy Processors,Processed,Blocked,QueueSize,Attempt\n";
    for (const auto & log : logs) {
        myfile << log;
    }
    myfile.close();
}
