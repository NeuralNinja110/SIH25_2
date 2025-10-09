/**
 * @file FileUtils.h
 * @brief File utility functions
 * @version 1.0.0
 * @date 2025-10-09
 */

#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <string>
#include <cstdint>

namespace obfuscator {

class FileUtils {
public:
    static bool fileExists(const std::string& filepath);
    static uint64_t getFileSize(const std::string& filepath);
    static bool createDirectory(const std::string& path);
    static std::string getFileExtension(const std::string& filepath);
    static std::string getBaseName(const std::string& filepath);
    static std::string getDirectory(const std::string& filepath);
    static bool copyFile(const std::string& src, const std::string& dest);
    static bool deleteFile(const std::string& filepath);
    static std::string readFile(const std::string& filepath);
    static bool writeFile(const std::string& filepath, const std::string& content);
};

} // namespace obfuscator

#endif // FILE_UTILS_H
