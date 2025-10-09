/**
 * @file CLIParser.h
 * @brief Command-line interface parser
 * @version 1.0.0
 * @date 2025-10-09
 */

#ifndef CLI_PARSER_H
#define CLI_PARSER_H

#include <string>
#include <vector>
#include "ObfuscationConfig.h"

namespace obfuscator {

class CLIParser {
public:
    CLIParser();
    ~CLIParser();

    bool parse(int argc, char* argv[]);
    const ObfuscationConfig& getConfig() const { return config_; }
    const std::string& getInputFile() const { return inputFile_; }
    const std::string& getOutputFile() const { return outputFile_; }
    bool shouldShowHelp() const { return showHelp_; }
    bool shouldShowVersion() const { return showVersion_; }

    void printHelp() const;
    void printVersion() const;

private:
    ObfuscationConfig config_;
    std::string inputFile_;
    std::string outputFile_;
    bool showHelp_;
    bool showVersion_;

    void setDefaults();
    bool parseArgument(const std::string& arg, const std::string& value);
};

} // namespace obfuscator

#endif // CLI_PARSER_H
