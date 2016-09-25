#include <iostream>
#include "huffman.h"
#include "argparser.h"

int main(int argc, char *argv[]) {
    ArgParser parser;
    parser.parse(argc - 1, argv + 1);

    bool corr_keys = (parser.is_key_sh('c') || parser.is_key_sh('u')) &&
                     (parser.is_key_sh('f') || parser.is_key_lg("file")) &&
                     (parser.is_key_sh('o') || parser.is_key_lg("output"));
    std::string fin = std::max(parser.get_arg_sh('f'), parser.get_arg_lg("file"));
    std::string fout = std::max(parser.get_arg_sh('o'), parser.get_arg_lg("output"));

    if(argc != 6 || !corr_keys || fin == "" || fout == "") {
        std::cout << "Incorrect program arguments.\n"
                  << "Usage: ./main -c/-u -f input_file -o output_file";
        return 1;
    }

    Huffman huff;
    try {
        if(parser.is_key_sh('c'))
            huff.zip(fin, fout);
        else
            huff.unzip(fin, fout);
        huff.print_stats();
    }
    catch(...) {
        std::cout << "Program failed.\nCheck whether the input file is correct.";
    }

    return 0;
}
