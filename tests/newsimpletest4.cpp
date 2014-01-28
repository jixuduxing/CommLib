/* 
 * File:   newsimpletest4.cpp
 * Author: Administrator
 *
 * Created on 2014-1-27, 10:47:59
 */

#include <stdlib.h>
#include <iostream>
#include "ThreadLoop.h"
#include "Time.h"
using namespace CommLib;
/*
 * Simple C++ Test Suite
 */
class TestLoop : public ThreadLoop {
public:

    TestLoop(int looptime) : ThreadLoop("TestLoop", looptime) {
    }

    virtual void LoopTask() {
        std::cout << this->ThreadName() << " Time:" << Time::GetCurrTick() << std::endl;
    }

};

void test1() {
    
    TestLoop loop( 5 );
    loop.StartThread();
    
    sleep(60);
    loop.StopLoop();
    loop.Join();
    
    std::cout << "newsimpletest4 test 1" << std::endl;
}

void test2() {
    std::cout << "newsimpletest4 test 2" << std::endl;
//    std::cout << "%TEST_FAILED% time=0 testname=test2 (newsimpletest4) message=error message sample" << std::endl;
}

int main(int argc, char** argv) {
    std::cout << "%SUITE_STARTING% newsimpletest4" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% test1 (newsimpletest4)" << std::endl;
    test1();
    std::cout << "%TEST_FINISHED% time=0 test1 (newsimpletest4)" << std::endl;

    std::cout << "%TEST_STARTED% test2 (newsimpletest4)\n" << std::endl;
    test2();
    std::cout << "%TEST_FINISHED% time=0 test2 (newsimpletest4)" << std::endl;

    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}

