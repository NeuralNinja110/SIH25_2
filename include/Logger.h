/**
 * @file Logger.h
 * @brief Logging utility
 * @version 1.0.0
 * @date 2025-10-09
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <mutex>

namespace obfuscator {

enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

class Logger {
public:
    static Logger& getInstance();
    
    void setLogLevel(LogLevel level);
    void setLogFile(const std::string& filepath);
    void setVerbose(bool verbose);
    
    void debug(const std::string& message);
    void info(const std::string& message);
    void warning(const std::string& message);
    void error(const std::string& message);

private:
    Logger();
    ~Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    void log(LogLevel level, const std::string& message);
    std::string levelToString(LogLevel level) const;
    std::string getCurrentTimestamp() const;

    LogLevel logLevel_;
    bool verbose_;
    std::ofstream logFile_;
    std::mutex mutex_;
};

} // namespace obfuscator

#endif // LOGGER_H
