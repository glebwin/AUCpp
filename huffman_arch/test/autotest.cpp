#include <climits>
#include <iostream>
#include <fstream>
#include "autotest.h"
#include "../src/huffman.h"

Autotest::Autotest()
    : cur_test(1) {
}

void Autotest::test() {
    test_abc();
    test_one_symb();
    test_max1();
    test_max2();
}

void Autotest::start_test() {
    std::cerr << "Test " << cur_test++ << ": ";
}

void Autotest::end_test(bool res) {
    if(res)
        std::cerr << "OK\n";
    else
        std::cerr << "FAIL\n";
}

bool Autotest::cmp_files(const std::string &fname1, const std::string &fname2) {
    std::ifstream file1(fname1);
    std::ifstream file2(fname2);

    const int buff_cap = 1 << 20;
    std::string buff1;
    std::string buff2;
    buff1.resize(buff_cap);
    buff2.resize(buff_cap);

    while(!file1.eof() && !file2.eof()) {
        file1.read(&buff1[0], buff_cap);
        file2.read(&buff2[0], buff_cap);
        if(file1.eof())
            buff1.resize(file1.gcount());
        if(file2.eof())
            buff2.resize(file2.gcount());

        if(buff1 != buff2)
            return false;
    }

    return file1.eof() && file2.eof();
}

void Autotest::zip_unzip(const std::string &fname1, const std::string &fname2) {
    Huffman huff;
    huff.zip(fname1, "test_temp.txt");
    huff.unzip("test_temp.txt", fname2);
    remove("test_temp.txt");
}

void Autotest::test_abc() {
    start_test();

    std::ofstream fout("test_in.txt");
    for(int i = 0; i < (1 << CHAR_BIT); i++) {
        char x = i;
        fout.write(&x, 1);
    }
    fout.close();

    zip_unzip("test_in.txt", "test_out.txt");

    end_test(cmp_files("test_in.txt", "test_out.txt"));
    remove("test_in.txt");
    remove("test_out.txt");
}

void Autotest::test_one_symb() {
    start_test();

    std::ofstream fout("test_in.txt");
    for(int i = 0; i < 1000; i++) {
        char x = 'a';
        fout.write(&x, 1);
    }
    fout.close();

    zip_unzip("test_in.txt", "test_out.txt");

    end_test(cmp_files("test_in.txt", "test_out.txt"));
    remove("test_in.txt");
    remove("test_out.txt");
}

void Autotest::test_max1() {
    start_test();

    std::ofstream fout("test_in.txt");
    for(int i = 0; i < (5 << 20); i++) {
        char x = i % (1 << CHAR_BIT - 1);
        fout.write(&x, 1);
    }
    fout.close();

    zip_unzip("test_in.txt", "test_out.txt");

    end_test(cmp_files("test_in.txt", "test_out.txt"));
    remove("test_in.txt");
    remove("test_out.txt");
}

void Autotest::test_max2() {
    start_test();

    std::ofstream fout("test_in.txt");
    for(int i = 0; i < (5 << 20); i++) {
        char x = i % (1 << CHAR_BIT);
        fout.write(&x, 1);
    }
    fout.close();

    zip_unzip("test_in.txt", "test_out.txt");

    end_test(cmp_files("test_in.txt", "test_out.txt"));
    remove("test_in.txt");
    remove("test_out.txt");
}
