#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <string>
#include <vector>

class Reader;
class Writer;

class Huffman {
private:

    static const int RET_SUCC;
    static const int RET_ERR;
    static const int MAX_FILE_SZ;
    static const int BUFF_SZ;
    static int MAX_FILE_SZ_LG;

    int in_bits;
    int out_bits;
    int info_bits;

    std::string fin_name;
    std::string fout_name;

    std::vector<int> freq;
    std::vector<std::pair<int, unsigned char>> inc_freq;
    std::vector<std::pair<int, int>> tree;
    std::vector<unsigned char> leaves;
    std::vector<std::vector<bool>> codes;

public:

    Huffman();
    int run(int argc, char *argv[]);
    void zip(const std::string &file_in, const std::string &file_out);
    void unzip(const std::string &file_in, const std::string &file_out);
    void print_stats();

private:

    void init(const std::string &file_in, const std::string &file_out);
    void count_freq();
    void sort_freq();
    void build_tree();
    void calc_codes(int v, std::vector<bool> &code);
    void encrypt();
    void write_tree(int v, Writer &writer);
    void read_tree(int v, Reader &reader);
    void calc_zip_stats();
    bool in_exists();
    bool in_empty();
};


#endif
