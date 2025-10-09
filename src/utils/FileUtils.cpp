/**
 * @file FileUtils.cpp
 * @brief Implementation of FileUtils
 * @version 1.0.0
 * @date 2025-10-09
 */

#include "FileUtils.h"
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstring>

namespace obfuscator {

bool FileUtils::fileExists(const std::string& filepath) {
    struct stat buffer;
    return (stat(filepath.c_str(), &buffer) == 0);
}

uint64_t FileUtils::getFileSize(const std::string& filepath) {
    struct stat buffer;
    if (stat(filepath.c_str(), &buffer) != 0) {
        return 0;
    }
    return static_cast<uint64_t>(buffer.st_size);
}

bool FileUtils::createDirectory(const std::string& path) {
#ifdef _WIN32
    return mkdir(path.c_str()) == 0;
#else
    return mkdir(path.c_str(), 0755) == 0;
#endif
}

std::string FileUtils::getFileExtension(const std::string& filepath) {
    size_t pos = filepath.find_last_of('.');
    if (pos == std::string::npos) {
        return "";
    }
    return filepath.substr(pos);
}

std::string FileUtils::getBaseName(const std::string& filepath) {
    size_t pos = filepath.find_last_of("/\\");
    if (pos == std::string::npos) {
        return filepath;
    }
    return filepath.substr(pos + 1);
}

std::string FileUtils::getDirectory(const std::string& filepath) {
    size_t pos = filepath.find_last_of("/\\");
    if (pos == std::string::npos) {
        return ".";
    }
    return filepath.substr(0, pos);
}

bool FileUtils::copyFile(const std::string& src, const std::string& dest) {
    std::ifstream source(src, std::ios::binary);
    if (!source) {
        return false;
    }
    
    std::ofstream destination(dest, std::ios::binary);
    if (!destination) {
        return false;
    }
    
    destination << source.rdbuf();
    return true;
}

bool FileUtils::deleteFile(const std::string& filepath) {
    return std::remove(filepath.c_str()) == 0;
}

std::string FileUtils::readFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file) {
        return "";
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

bool FileUtils::writeFile(const std::string& filepath, const std::string& content) {
    std::ofstream file(filepath);
    if (!file) {
        return false;
    }
    
    file << content;
    return file.good();
}

} // namespace obfuscator
