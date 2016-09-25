#ifndef AUTOTEST_H
#define AUTOTEST_H

#include <string>

class Autotest {
private:

    int cur_test;

public:

    Autotest();
    void test();

private:

    void start_test();
    void end_test(bool res);
    bool cmp_files(const std::string &fname1, const std::string &fname2);
    void zip_unzip(const std::string &fname1, const std::string &fname2);

    void test_abc();
    void test_one_symb();
    void test_max1();
    void test_max2();
};

#endif
