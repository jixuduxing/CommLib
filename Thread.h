/* 
 * File:   Thread.h
 * Author: Administrator
 *
 * Created on 2014年1月23日, 上午10:27
 */

#ifndef THREAD_H
#define	THREAD_H

#include <pthread.h>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <string>

namespace CommLib {
    class Thread;

    class ThreadData : boost::noncopyable {
    protected:

        ThreadData(std::string name, boost::function<void () > func)
        : pID_(0), pHandle_(0)
        , name_(name), func_(func) {
        };

        std::string name_;
        pthread_t pHandle_;
        int pID_;
        friend class Thread;

    public:        
         boost::function<void () > func_;

    };

    class Thread : boost::noncopyable {
    public:
        Thread( boost::function<void () > func,std::string name="");
        virtual ~Thread();

        void StartThread();
        void Join();

        int Thread_id();
        std::string ThreadName();

    private:
        ThreadData ThrData_;

    };
}
#endif	/* THREAD_H */

