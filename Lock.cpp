#include "Lock.h"
#include <assert.h>
#include <bits/time.h>
#include <sys/time.h>

#define CHECK(ret) ({ __typeof__ (ret) errnum = (ret);         \
                       assert(errnum == 0); (void) errnum;})

namespace CommLib {

    CMutexLock::CMutexLock() {
        CHECK(pthread_mutex_init(&mutex_, NULL));
    }

    CMutexLock::~CMutexLock() {
        CHECK(pthread_mutex_destroy(&mutex_));
    }

    void CMutexLock::Lock() {
        CHECK(pthread_mutex_lock(&mutex_));
    }

    void CMutexLock::UnLock() {
        CHECK(pthread_mutex_unlock(&mutex_));
    }

    bool CMutexLock::TryLock() {
        return ( 0 == pthread_mutex_trylock(&mutex_));
    }

    CAutoLock::CAutoLock(CMutexLock& lock) : Lock_(lock) {
        Lock_.Lock();
    }

    CAutoLock::~CAutoLock() {
        Lock_.UnLock();
    }

    RwLock::RwLock() {
        CHECK(pthread_rwlock_init(&rwlock_, NULL));

    }

    RwLock::~RwLock() {
        CHECK(pthread_rwlock_destroy(&rwlock_));
    }

    void RwLock::RdLock() {
        CHECK(pthread_rwlock_rdlock(&rwlock_));
    }

    void RwLock::WrLock() {
        CHECK(pthread_rwlock_wrlock(&rwlock_));
    }

    void RwLock::UnLock() {
        CHECK(pthread_rwlock_unlock(&rwlock_));
    }

    bool RwLock::TryRdLock() {
        return ( 0 == pthread_rwlock_tryrdlock(&rwlock_));
    }

    bool RwLock::TryWrLock() {
        return (0 == pthread_rwlock_trywrlock(&rwlock_));
    }

    Condition::Condition() {
        CHECK(pthread_cond_init(&cond_, NULL));
    }

    Condition::~Condition() {
        CHECK(pthread_cond_destroy(&cond_));
    }

    void Condition::Signal() {
        CHECK(pthread_cond_signal(&cond_));
    }

    void Condition::Broadcast() {
        CHECK(pthread_cond_broadcast(&cond_));
    }

    void Condition::Wait() {
        CAutoLock al(Lock_);
        CHECK(pthread_cond_wait(&cond_, &Lock_.GetMutex()));
    }

    int Condition::TimeWait(int sec) {
        CAutoLock al(Lock_);

        timeval now;
        gettimeofday(&now, NULL);

        timespec spec;
        spec.tv_sec = now.tv_sec + sec;
        spec.tv_nsec = now.tv_usec * 1000;
        int iret = pthread_cond_timedwait(&cond_, &Lock_.GetMutex(), &spec);

        return iret;
    }
}
