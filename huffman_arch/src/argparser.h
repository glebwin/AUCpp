#ifndef ARGPARSER_H
#define ARGPARSER_H

#include <string>
#include <unordered_map>

class ArgParser {
private:

    std::unordered_map<char, std::string> args_sh;
    std::unordered_map<std::string, std::string> args_lg;

public:

    void parse(int argc, char *argv[]);
    bool is_key_sh(char key);
    bool is_key_lg(const std::string &key);
    std::string get_arg_sh(char key);
    std::string get_arg_lg(const std::string &key);
};

#endif
