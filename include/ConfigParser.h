/**
 * @file ConfigParser.h
 * @brief Configuration file parser
 * @version 1.0.0
 * @date 2025-10-09
 */

#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include <string>
#include "ObfuscationConfig.h"

namespace obfuscator {

class ConfigParser {
public:
    ConfigParser();
    ~ConfigParser();

    bool parseFile(const std::string& filepath, ObfuscationConfig& config);
    bool parseJSON(const std::string& json, ObfuscationConfig& config);

private:
    bool validateJSON(const std::string& json);
};

} // namespace obfuscator

#endif // CONFIG_PARSER_H
