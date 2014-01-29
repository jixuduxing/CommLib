/* 
 * File:   ThreadPool.cpp
 * Author: Administrator
 * 
 * Created on 2014年1月24日, 上午10:04
 */
#include <boost/function.hpp>
#include <boost/bind.hpp>

#include "ThreadPool.h"
#include "Semaphore.h"
namespace CommLib {
ThreadPool::ThreadPool() : bRuning_(false) ,sem_(0,0){
}

ThreadPool::~ThreadPool() {
    ClearTaskList();
    Stop();
}

void ThreadPool::AddTask(boost::function<void () > func) {

    {
        CAutoLock al(lock_);
        tasklist_.push_back(func);
    }

    sem_.post();
}

int ThreadPool::GetCurrTaskSize() {
    CAutoLock al(lock_);
    return tasklist_.size();
}

bool ThreadPool::PopTask(boost::function<void () >& func) {
    sem_.wait();

    CAutoLock al(lock_);

    if (!tasklist_.empty()) {
        func = tasklist_.front();
        tasklist_.pop_front();
        return true;
    } else
        return false;
}

void ThreadPool::ThreadFunc() {
    while ( bRunning() ) {
        boost::function<void () > func;

        while (PopTask(func)) {
            func();
        }
    }
}

bool ThreadPool::Init(int threadCnt) {
    CAutoLock al(lockOp_);

    if ((!bRuning_) &&(threadCnt < MAX_THREADPOOLSIZE)) {
        threadvec_.resize(threadCnt);
        bRuning_ = true;
        for (int i = 0; i < threadCnt; i++) {
            threadvec_[i] = new Thread("", boost::bind(&ThreadPool::ThreadFunc, this));
            threadvec_[i]->StartThread();
        }

        return true;
    }
    return false;
}

void ThreadPool::Stop() {
    CAutoLock al(lockOp_);

    if (bRuning_) {
        bRuning_ = false;
//        con_.Broadcast();

        int threadCnt = threadvec_.size();
        
        for(int i = 0; i < threadCnt; i++)
        {
            sem_.post();
        }
        
        for (int i = 0; i < threadCnt; i++) {
            threadvec_[i]->Join();
            delete threadvec_[i];
        }

        threadvec_.clear();
    }
}
}
