/* 
 * File:   ThreadPool.h
 * Author: Administrator
 *
 * Created on 2014年1月24日, 上午10:04
 */

#ifndef THREADPOOL_H
#define	THREADPOOL_H

#include <list>
#include <vector>

#include "Thread.h"
#include "Lock.h"
#include "Semaphore.h"

#define MAX_THREADPOOLSIZE 100
namespace CommLib {

    class ThreadPool : boost::noncopyable {
    public:
        ThreadPool();
        virtual ~ThreadPool();

        void AddTask(boost::function<void () > func);
        int GetCurrTaskSize();

        void ClearTaskList() {
            CAutoLock al(lock_);
            tasklist_.clear();
        }

        void ThreadFunc();
        bool Init(int threadCnt);
        void Stop();

        bool bRunning() {
            return bRuning_;
        }

    private:

        bool PopTask(boost::function<void () >& func);
    private:
        bool bRuning_;
        CMutexLock lock_;
        CMutexLock lockOp_;
        std::vector<Thread*> threadvec_;
        std::list<boost::function<void () > > tasklist_;
        //   CCondition con_;

        Semaphore sem_;
    };
}
#endif	/* THREADPOOL_H */

