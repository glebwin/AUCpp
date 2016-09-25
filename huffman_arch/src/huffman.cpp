#include <algorithm>
#include <climits>
#include <cstring>
#include <fstream>
#include <iostream>
#include "huffman.h"
#include "reader.h"
#include "writer.h"

const int Huffman::RET_SUCC = 0;
const int Huffman::RET_ERR  = 1;
const int Huffman::MAX_FILE_SZ = (5 << 20) + (1 << 10);
int Huffman::MAX_FILE_SZ_LG = 0;
const int Huffman::BUFF_SZ = 1 << 20;

Huffman::Huffman() : freq(1 << CHAR_BIT), codes(1 << CHAR_BIT) {
    while(1 << MAX_FILE_SZ_LG <= MAX_FILE_SZ)
        MAX_FILE_SZ_LG++;
}

void Huffman::zip(const std::string &file_in, const std::string &file_out) {
    init(file_in, file_out);
    if(!in_exists())
        throw 0;
    if(in_empty()) {
        std::ofstream fout(file_out);
        return;
    }

    count_freq();
    sort_freq();
    if(inc_freq.size() > 1) {
        build_tree();
        std::vector<bool> code;
        calc_codes(tree.size() - 1, code);
    }
    else
        leaves.push_back(inc_freq[0].second);

    encrypt();
}

void Huffman::unzip(const std::string &file_in, const std::string &file_out) {
    init(file_in, file_out);
    if(!in_exists())
        throw 0;
    if(in_empty()) {
        std::ofstream fout(file_out);
        return;
    }

    Reader reader(fin_name, BUFF_SZ);
    while(!reader.read_bit())
        info_bits++;
    info_bits++;

    int abc_size = reader.read_byte();
    if(abc_size == 0)
        abc_size = 1 << CHAR_BIT;

    leaves.reserve(abc_size);
    for(int i = 0; i < abc_size; i++)
        leaves.push_back(reader.read_byte());

    info_bits += (1 + abc_size) * CHAR_BIT;

    Writer writer(fout_name, BUFF_SZ);

    if(abc_size == 1) {
        in_bits = MAX_FILE_SZ_LG;
        for(int i = 0; i < MAX_FILE_SZ_LG; i++)
            out_bits = (out_bits << 1) | reader.read_bit();
        for(int i = 0; i < out_bits; i++)
            writer.write_byte(leaves[0]);
        out_bits *= CHAR_BIT;
        return;
    }

    read_tree(0, reader);
    for(int i = 0, j = 0; i < tree.size(); i++)
        if(tree[i].first == -1)
            tree[i].second = leaves[j++];

    info_bits += tree.size();

    int v = 0;
    while(true) {
        bool bit = reader.read_bit();
        if(reader.eof())
            break;

        in_bits++;

        if(!bit)
            v = tree[v].first;
        else
            v = tree[v].second;

        if(tree[v].first == -1) {
            writer.write_byte(tree[v].second);
            out_bits++;
            v = 0;
        }
    }
    out_bits *= CHAR_BIT;
}

void Huffman::print_stats() {
    std::cout << in_bits / CHAR_BIT << '\n'
              << out_bits / CHAR_BIT << '\n'
              << (info_bits + in_bits % CHAR_BIT + out_bits % CHAR_BIT) / CHAR_BIT << '\n';
}

void Huffman::init(const std::string &file_in, const std::string &file_out) {
    fin_name = file_in;
    fout_name = file_out;
    in_bits = 0;
    out_bits = 0;
    info_bits = 0;
    std::fill(freq.begin(), freq.end(), 0);
    inc_freq.clear();
    tree.clear();
    leaves.clear();
}

void Huffman::count_freq() {
    Reader reader(fin_name, BUFF_SZ);
    while(true) {
        unsigned char x = reader.read_byte();
        if(reader.eof())
            break;
        freq[x]++;
    }
}

void Huffman::sort_freq() {
    for(size_t i = 0; i < (1 << CHAR_BIT); i++)
        if(freq[i])
            inc_freq.push_back(std::make_pair(freq[i], i));
    std::sort(inc_freq.begin(), inc_freq.end());
}

void Huffman::build_tree() {
    tree.reserve(inc_freq.size() * 2);

    for(std::pair<int, unsigned char> x : inc_freq)
        tree.push_back(std::make_pair(-1, x.second));

    // MAX_FILES_SZ is used here as an infinite value
    inc_freq.push_back(std::make_pair(Huffman::MAX_FILE_SZ, 0));
    inc_freq.push_back(std::make_pair(Huffman::MAX_FILE_SZ, 0));
    std::vector<std::pair<int, int>> nodes(inc_freq.size(),
                                           std::make_pair(MAX_FILE_SZ, 0));

    int p1 = 0, p2 = 0;
    int term_pos = 0;

    for(int k = 0; k < static_cast<int>(inc_freq.size()) - 3; k++) {
        int sum11 = inc_freq[p1].first + inc_freq[p1 + 1].first;
        int sum22 = nodes[p2].first + nodes[p2 + 1].first;
        int sum12 = inc_freq[p1].first + nodes[p2].first;

        if(sum11 <= sum22 && sum11 <= sum12) {
            nodes[k] = std::make_pair(sum11, tree.size());
            tree.push_back(std::make_pair(p1, p1 + 1));
            p1 += 2;
        }
        else if(sum22 <= sum12) {
            nodes[k] = std::make_pair(sum22, tree.size());
            if(term_pos == p2 || term_pos == p2 + 1) {
                if(term_pos == p2)
                    tree.push_back(std::make_pair(nodes[p2].second, nodes[p2 + 1].second));
                else
                    tree.push_back(std::make_pair(nodes[p2 + 1].second, nodes[p2].second));
                term_pos = k;
            }
            else
                tree.push_back(std::make_pair(nodes[p2].second, nodes[p2 + 1].second));
            p2 += 2;
        }
        else {
            nodes[k] = std::make_pair(sum12, tree.size());
            if(term_pos == p2)
                term_pos = k;
            tree.push_back(std::make_pair(nodes[p2].second, p1));
            p1++;
            p2++;
        }
    }

    inc_freq.pop_back();
    inc_freq.pop_back();
}

void Huffman::calc_codes(int v, std::vector<bool> &code) {
    if(tree[v].first == -1) {
        codes[tree[v].second] = code;
        leaves.push_back(tree[v].second);
        return;
    }

    code.push_back(0);
    calc_codes(tree[v].first,  code);
    code.back() = 1;
    calc_codes(tree[v].second, code);
    code.pop_back();
}

void Huffman::encrypt() {
    Writer writer(fout_name, BUFF_SZ);

    calc_zip_stats();

    int offset = CHAR_BIT - (out_bits + info_bits) % 8;
    info_bits += offset;
    for(int i = 0; i < offset - 1; i++)
        writer.write_bit(0);
    writer.write_bit(1);

    // 256 <=> 0
    writer.write_byte(leaves.size());
    for(int i = 0; i < leaves.size(); i++)
        writer.write_byte(leaves[i]);

    if(leaves.size() == 1) {
        int bytes = in_bits / CHAR_BIT;
        for(int i = MAX_FILE_SZ_LG - 1; i >= 0; i--)
            writer.write_bit(bytes & (1 << i));
        return;
    }

    write_tree(tree.size() - 1, writer);

    Reader reader(fin_name, BUFF_SZ);
    while(true) {
        unsigned char x = reader.read_byte();
        if(reader.eof())
            break;
        for(bool b : codes[x])
            writer.write_bit(b);
    }
}

void Huffman::write_tree(int v, Writer &writer) {
    if(tree[v].first != -1) {
        writer.write_bit(0);
        write_tree(tree[v].first, writer);
        write_tree(tree[v].second, writer);
    }
    else
        writer.write_bit(1);
}

void Huffman::read_tree(int v, Reader &reader) {
    if(!reader.read_bit()) {
        tree.push_back(std::make_pair(tree.size() + 1, 0));
        read_tree(tree[v].first, reader);
        tree[v].second = tree.size();
        read_tree(tree[v].second, reader);
    }
    else
        tree.push_back(std::make_pair(-1, -1));
}

void Huffman::calc_zip_stats() {
    Reader reader(fin_name, BUFF_SZ);

    while(true) {
        unsigned char x = reader.read_byte();
        if(reader.eof())
            break;

        in_bits++;
        out_bits += codes[x].size();
    }
    in_bits *= CHAR_BIT;
    info_bits = CHAR_BIT + leaves.size() * CHAR_BIT + tree.size();

    if(leaves.size() == 1)
        out_bits = MAX_FILE_SZ_LG;
}

bool Huffman::in_exists() {
    std::ifstream file(fin_name);
    return file.good();
}

bool Huffman::in_empty() {
    std::ifstream file(fin_name);
    return file.peek() == std::ifstream::traits_type::eof();
}
