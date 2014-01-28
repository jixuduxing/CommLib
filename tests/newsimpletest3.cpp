/* 
 * File:   newsimpletest3.cpp
 * Author: Administrator
 *
 * Created on 2014-1-27, 9:25:39
 */

#include <stdlib.h>
#include <iostream>

#include "Time.h"
using namespace CommLib;
/*
 * Simple C++ Test Suite
 */

void test1() {
    Time tm = Time::GetCurrentTime();
    std::cout<<tm.GetHour()<<":";
    std::cout<<tm.GetMinute()<<":";
    std::cout<<tm.GetSecond()<<" ";
    std::cout<<tm.GetYear()<<":";
    std::cout<<tm.GetMonth()<<":";
    std::cout<<tm.GetDay()<<"周";
    std::cout<<tm.GetDayOfWeek()<<" ";

    tm = tm + 28800*3;

    std::cout<<tm.GetHour()<<":";
    std::cout<<tm.GetMinute()<<":";
    std::cout<<tm.GetSecond()<<" ";
    std::cout<<tm.GetYear()<<":";
    std::cout<<tm.GetMonth()<<":";
    std::cout<<tm.GetDay()<<"周";
    std::cout<<tm.GetDayOfWeek()<<" ";
    std::cout << "newsimpletest3 test 1" << std::endl;
}

void test2() {
    std::cout << "newsimpletest3 test 2" << std::endl;
//    std::cout << "%TEST_FAILED% time=0 testname=test2 (newsimpletest3) message=error message sample" << std::endl;
}

int main(int argc, char** argv) {
    std::cout << "%SUITE_STARTING% newsimpletest3" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% test1 (newsimpletest3)" << std::endl;
    test1();
    std::cout << "%TEST_FINISHED% time=0 test1 (newsimpletest3)" << std::endl;

    std::cout << "%TEST_STARTED% test2 (newsimpletest3)\n" << std::endl;
    test2();
    std::cout << "%TEST_FINISHED% time=0 test2 (newsimpletest3)" << std::endl;

    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}

