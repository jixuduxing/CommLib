/* 
 * File:   newsimpletest.cpp
 * Author: Administrator
 *
 * Created on 2014-1-23, 17:25:08
 */

#include <stdlib.h>
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>

#include "../Thread.h"
#include "../MemPool.h"
using namespace CommLib;
/*
 * Simple C++ Test Suite
 */

void test1() {
    std::cout << "newsimpletest test 1" << std::endl;
}

void test2() {
    std::cout << "newsimpletest test 2" << std::endl;
    std::cout << "%TEST_FAILED% time=0 testname=test2 (newsimpletest) message=error message sample" << std::endl;
}

void threadFun( boost::shared_ptr<MemPool> mp) {
    std::cout << "this thread:" << pthread_self() << std::endl;
    //    return;
    srand(10000);
    for (int i = 1; i < 10000; i += 1) {
//        int count = rand()%10000000000;
        int count = i;
        AllocPack* pPack = mp->Alloc(count);

        if (pPack) {
//            if (i % 127) {
                mp->Free(pPack);
            }
//        } else {
//            std::cout << "over" << std::endl;
//            break;
//        }
    }
    mp->PrintfSelf(std::cout);
}


int main() {
    boost::shared_ptr<MemPool> mp(new MemPool(MemPool::SimpleList_Mp));
    
    typedef boost::function<void () > MyFun;
    MyFun func = boost::bind(&threadFun, mp);

    Thread thr1(func,"1");
    Thread thr2(func,"2");
    Thread thr3(func,"3");
    Thread thr4(func,"4");

//    thr1.SetFunc(func);
//    thr2.SetFunc(func);
//    thr3.SetFunc(func);
//    thr4.SetFunc(func);

    thr1.StartThread();
    thr2.StartThread();
    thr3.StartThread();
    thr4.StartThread();

    thr1.Join();
    thr2.Join();
    thr3.Join();
    thr4.Join();

    return 1;
}


int main2(int argc, char** argv) {
    std::cout << "%SUITE_STARTING% newsimpletest" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% test1 (newsimpletest)" << std::endl;
    test1();
    std::cout << "%TEST_FINISHED% time=0 test1 (newsimpletest)" << std::endl;

    std::cout << "%TEST_STARTED% test2 (newsimpletest)\n" << std::endl;
    test2();
    std::cout << "%TEST_FINISHED% time=0 test2 (newsimpletest)" << std::endl;

    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}

