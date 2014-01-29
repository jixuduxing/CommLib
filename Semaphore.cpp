/* 
 * File:   Semaphore.cpp
 * Author: Administrator
 * 
 * Created on 2014年1月26日, 下午3:21
 */

#include "Semaphore.h"
namespace CommLib {

    Semaphore::Semaphore(int pshared, int value) {
        sem_init(&semt_, pshared, value);
    }

    Semaphore::~Semaphore() {
        sem_destroy(&semt_);
    }

    void Semaphore::post() {
        sem_post(&semt_);
    }

    void Semaphore::wait() {
        sem_wait(&semt_);
    }

}

