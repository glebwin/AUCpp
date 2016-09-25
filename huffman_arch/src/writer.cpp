#include <climits>
#include "writer.h"

Writer::Writer(const std::string &file_name, size_t buff_cap)
    : file(file_name, std::ofstream::out | std::ofstream::binary),
      buff_cap(buff_cap), cur_byte(0), cur_byte_cnt(0) {
    buff.reserve(buff_cap);
}

Writer::~Writer() {
    if(cur_byte_cnt) {
        cur_byte <<= CHAR_BIT - cur_byte_cnt;
        buff.push_back(cur_byte);
    }
    flush();
}

void Writer::write_bit(bool bit) {
    cur_byte = (cur_byte << 1) | bit;
    cur_byte_cnt++;
    if(cur_byte_cnt == CHAR_BIT) {
        buff.push_back(cur_byte);
        cur_byte_cnt = 0;

        if(buff.size() == buff_cap)
            flush();
    }
}

void Writer::write_byte(char byte) {
    for(int i = CHAR_BIT - 1; i >= 0; i--)
        write_bit(byte & (1 << i));
}

void Writer::flush() {
    file.write(reinterpret_cast<char*>(&buff.front()), buff.size());
    buff.clear();
}
