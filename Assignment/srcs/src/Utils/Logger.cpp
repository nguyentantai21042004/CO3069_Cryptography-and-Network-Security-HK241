#include "logger.h"
#include <iostream>
#include <unordered_map>

namespace Logger
{
    static std::unordered_map<std::string, std::chrono::high_resolution_clock::time_point> timers;

    void startTimer(const std::string &label)
    {
        timers[label] = std::chrono::high_resolution_clock::now();
    }

    void endTimer(const std::string &label)
    {
        auto end_time = std::chrono::high_resolution_clock::now();
        if (timers.find(label) != timers.end())
        {
            auto start_time = timers[label];
            std::chrono::duration<double> elapsed = end_time - start_time;
            std::cout << "[" << label << "] Time taken: " << elapsed.count() << " seconds\n";
            timers.erase(label);
        }
        else
        {
            std::cerr << "Timer for label '" << label << "' was not started.\n";
        }
    }

    void log(const std::string &message)
    {
        std::cout << message << std::endl;
    }

    void log(const std::string &label, const std::string &message)
    {
        std::cout << "[" << label << "] " << message << std::endl;
    }
}
