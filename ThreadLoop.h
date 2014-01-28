/* 
 * File:   ThreadLoop.h
 * Author: Administrator
 *
 * Created on 2014年1月27日, 上午10:09
 */

#ifndef THREADLOOP_H
#define	THREADLOOP_H

#include "Thread.h"
#include "Lock.h"
namespace CommLib {

    class ThreadLoop : public Thread {
    public:
        ThreadLoop(std::string name, int loopTime);
        virtual ~ThreadLoop();

        virtual void LoopTask() = 0;

        void StopLoop() {
            con_.Signal();
        }
        void ThreadFunc();
    private:
        int loopTime_;

        Condition con_;

    };
}
#endif	/* CTHREADLOOP_H */

