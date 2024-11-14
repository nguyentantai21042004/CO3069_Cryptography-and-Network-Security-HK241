#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <chrono>

namespace Logger
{
    void startTimer(const std::string &label);
    void endTimer(const std::string &label);
    void log(const std::string &message);
    void log(const std::string &label, const std::string &message);
}

#endif // LOGGER_H
