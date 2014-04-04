/* 
 * File:   ThreadLoop.cpp
 * Author: Administrator
 * 
 * Created on 2014年1月27日, 上午10:09
 */
#include <boost/bind.hpp>
#include "ThreadLoop.h"
namespace CommLib {

    ThreadLoop::ThreadLoop(std::string name, int loopTime)
    : Thread( boost::bind(&ThreadLoop::ThreadFunc, this),name)
    , loopTime_(loopTime) {
    }

    ThreadLoop::~ThreadLoop() {
    }

    void ThreadLoop::ThreadFunc() {
        while (ETIMEDOUT == con_.TimeWait(loopTime_)) {
            LoopTask();
        }
    }
}
