#ifndef READER_H
#define READER_H

#include <fstream>
#include <string>
#include <vector>

class Reader {
private:

    std::ifstream file;
    std::vector<unsigned char> buff;
    std::vector<unsigned char>::iterator buff_it;
    size_t buff_cap;
    signed char cur_byte_cnt;
    bool is_eof;

public:

    Reader(std::string file_name, size_t buff_cap);
    bool read_bit();
    unsigned char read_byte();
    bool eof();

private:

    void read_buff();
};

#endif
