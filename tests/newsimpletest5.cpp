/* 
 * File:   newsimpletest5.cpp
 * Author: Administrator
 *
 * Created on 2014-1-27, 13:41:15
 */

#include <stdlib.h>
#include <iostream>
#include <boost/noncopyable.hpp>
#include <boost/pool/pool.hpp>
#include <boost/pool/singleton_pool.hpp>
#include <bitset>
std::bitset<2> bset;

class CopyAble:boost::noncopyable
{
public:
    int a_;
};
/*
 * Simple C++ Test Suite
 */

void test(CopyAble a)
{
    a.a_ = 2;
}

void test1() {
    CopyAble ca;
    ca.a_ = 1;
    
//    CopyAble ca2 = ca;
//    CopyAble ca3( ca );
//    test( ca );
    
    std::cout << "newsimpletest5 test 1" << std::endl;
}

void test2() {
    std::cout << "newsimpletest5 test 2" << std::endl;
//    std::cout << "%TEST_FAILED% time=0 testname=test2 (newsimpletest5) message=error message sample" << std::endl;
}

int main(int argc, char** argv) {
    std::cout << "%SUITE_STARTING% newsimpletest5" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% test1 (newsimpletest5)" << std::endl;
    test1();
    std::cout << "%TEST_FINISHED% time=0 test1 (newsimpletest5)" << std::endl;

    std::cout << "%TEST_STARTED% test2 (newsimpletest5)\n" << std::endl;
    test2();
    std::cout << "%TEST_FINISHED% time=0 test2 (newsimpletest5)" << std::endl;

    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}

