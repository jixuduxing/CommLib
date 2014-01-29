/* 
 * File:   Thread.cpp
 * Author: Administrator
 * 
 * Created on 2014年1月23日, 上午10:27
 */

#include "Thread.h"
namespace CommLib {

    Thread::Thread(std::string name, boost::function<void () > func)
    : ThrData_(name, func) {
    }

    Thread::~Thread() {
    }


    void* ThreadFunc(void* arg) {
        ThreadData* pThr = static_cast<ThreadData*> (arg);
        pThr->func_();
    }

    void Thread::StartThread() {
        ThrData_.pID_ = pthread_create(&ThrData_.pHandle_, NULL, ThreadFunc, &ThrData_);
    }

    void Thread::Join() {
        pthread_join(ThrData_.pHandle_, NULL);
    }

    int Thread::Thread_id() {
        return ThrData_.pID_;
    }

    std::string Thread::ThreadName() {
        return ThrData_.name_;
    }
}
