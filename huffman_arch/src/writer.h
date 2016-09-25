#ifndef WRITER_H
#define WRITER_H

#include <fstream>
#include <vector>

class Writer {
private:

    std::ofstream file;
    std::vector<unsigned char> buff;
    size_t buff_cap;
    char cur_byte;
    char cur_byte_cnt;

public:

    Writer(const std::string &file_name, size_t buff_sz);
    ~Writer();
    void write_bit(bool bit);
    void write_byte(char byte);
    void flush();
};

#endif
