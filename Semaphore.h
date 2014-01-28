/* 
 * File:   Semaphore.h
 * Author: Administrator
 *
 * Created on 2014年1月26日, 下午3:21
 */

#ifndef SEMAPHORE_H
#define	SEMAPHORE_H

#include <boost/noncopyable.hpp>
#include <semaphore.h>
namespace CommLib {

    class Semaphore : boost::noncopyable {
    public:
        Semaphore(int pshared, int value);

        virtual ~Semaphore();

        void post();
        void wait();

    private:
        sem_t semt_;

    };
}
#endif	/* SEMAPHORE_H */

