#include "argparser.h"

void ArgParser::parse(int argc, char *argv[]) {
    char key_sh;
    std::string key_lg;

    for(int i = 0; i < argc; i++)
        if(argv[i][0] == '-') {
            if(argv[i][1] != '-') {
                key_sh = argv[i][1];
                args_sh[key_sh] = "";
            }
            else {
                key_lg.assign(argv[i] + 2);
                args_lg[key_lg] = "";
            }
        }
        else if(i && argv[i - 1][0] == '-') {
            if(argv[i - 1][1] != '-')
                args_sh[key_sh] = argv[i];
            else
                args_lg[key_lg] = argv[i];
        }
}

bool ArgParser::is_key_sh(char key) {
    return args_sh.find(key) != args_sh.end();
}

bool ArgParser::is_key_lg(const std::string &key) {
    return args_lg.find(key) != args_lg.end();
}

std::string ArgParser::get_arg_sh(char key) {
    return args_sh[key];
}

std::string ArgParser::get_arg_lg(const std::string &key) {
    return args_lg[key];
}
