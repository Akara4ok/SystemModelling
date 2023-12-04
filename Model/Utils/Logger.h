//
// Created by vlad on 10/11/23.
//

#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <string>
#include <vector>
#include "optional"

class Logger {
public:
    Logger(std::string filename, bool saveLogArray = true);

    static void log(double currentTime, const std::string& name = "", const std::string& event = "",
                    std::optional<int> processing = std::nullopt, std::optional<int> processed = std::nullopt,
                    std::optional<int> blocked = std::nullopt, std::optional<int> queueSize = std::nullopt,
                    std::optional<int> attempt = std::nullopt);

    static void saveLogFile();

private:
    static std::string filename;
    static bool saveLogArray;
    static std::vector<std::string> logs;
};


#endif //LAB2_LOGGER_H
