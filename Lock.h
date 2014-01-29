/* 
 * File:   Lock.h
 * Author: Administrator
 *
 * Created on 2014年1月23日, 下午5:21
 */

#ifndef LOCK_H
#define	LOCK_H

#include <boost/noncopyable.hpp>
#include <pthread.h>

namespace CommLib {
    //临界区

    class CMutexLock : boost::noncopyable {
    public:
        CMutexLock();
        ~CMutexLock();

        void Lock();
        void UnLock();

        bool TryLock();

        pthread_mutex_t& GetMutex() {
            return mutex_;
        }

    private:
        pthread_mutex_t mutex_;
    };

    //自旋锁

    class CAutoLock : boost::noncopyable {
    public:
        CAutoLock(CMutexLock& lock);
        ~CAutoLock();

    private:
        CMutexLock& Lock_;
    };

    //读写锁

    class RwLock : boost::noncopyable {
    public:
        RwLock();
        ~RwLock();

        void RdLock();
        void WrLock();
        void UnLock();

        bool TryRdLock();
        bool TryWrLock();

    private:
        pthread_rwlock_t rwlock_;
    };

    //事件

    class Condition : boost::noncopyable {
    public:
        Condition();
        ~Condition();

        void Signal();
        void Broadcast();
        void Wait();

        int TimeWait(int sec);

    private:

        CMutexLock Lock_;
        pthread_cond_t cond_;
    };
}
#endif	/* LOCK_H */

