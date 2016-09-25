#include <climits>
#include "reader.h"

Reader::Reader(std::string file_name, size_t buff_cap)
    : file(file_name, std::ifstream::in | std::ifstream::binary),
      buff_cap(buff_cap), cur_byte_cnt(CHAR_BIT - 1), is_eof(false) {
    buff.resize(buff_cap);
    buff_it = buff.end();
}

bool Reader::read_bit() {
    if(eof())
        throw 0;
    if(buff_it == buff.end()) {
        read_buff();
        if(eof())
            return 0;
    }

    bool res = *buff_it & (1 << cur_byte_cnt--);

    if(cur_byte_cnt < 0) {
        cur_byte_cnt = CHAR_BIT - 1;
        ++buff_it;
    }

    return res;
}

unsigned char Reader::read_byte() {
    if(eof())
        throw 0;
    if(buff_it == buff.end()) {
        read_buff();
        if(eof())
            return 0;
    }

    unsigned char res = *(buff_it++) << (CHAR_BIT - 1 - cur_byte_cnt);

    if(cur_byte_cnt < 7) {
        if(buff_it == buff.end())
            read_buff();

        res |= *buff_it >> ((int)cur_byte_cnt + 1);
    }

    return res;
}

bool Reader::eof() {
    return is_eof;
}

void Reader::read_buff() {
    file.read(reinterpret_cast<char*>(&buff.front()), buff_cap);
    if(file.eof()) {
        if(!file.gcount()) {
            is_eof = true;
            return;
        }
        buff.resize(file.gcount());
    }
    buff_it = buff.begin();
    cur_byte_cnt = CHAR_BIT - 1;
}
