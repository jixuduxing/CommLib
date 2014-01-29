/* 
 * File:   newsimpletest1.cpp
 * Author: Administrator
 *
 * Created on 2014-1-24, 11:13:06
 */

#include <stdlib.h>
#include <iostream>

#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>

#include "../ThreadPool.h"
#include "../MemPool.h"

using namespace CommLib;

void threadFun( boost::shared_ptr<MemPool> mp) {
    std::cout << "this thread:" << pthread_self() << std::endl;
    //    return;
    srand(10000);
    for (int i = 1; i < 1000; i += 1) {
//        int count = rand()%10000000000;
        int count = i;
        char* buff = new char[i];
        
        if(buff)
            delete buff;
    }
 //   mp->printfself(std::cout);
}

#include <sys/time.h>

int main() {
    timeval tm;
    gettimeofday(&tm,NULL);
    std::cout<<"m:"<<tm.tv_sec<<" hm"<< tm.tv_usec <<std::endl;

    boost::shared_ptr<MemPool> mp(new MemPool(1));
    
//    typedef boost::function<void () > MyFun;
//    MyFun func = boost::bind(&threadFun, mp);
    ThreadPool thrpool;

    thrpool.Init(99);
    sleep(1);
    
    for( int i =0;i<100000;i++ )
       thrpool.AddTask(boost::bind(&threadFun, mp));
    
    thrpool.Stop();

    mp->PrintfSelf(std::cout);
    timeval tm2;
    gettimeofday(&tm2,NULL);
    
    std::cout<<"m:"<<(tm2.tv_sec - tm.tv_sec)<<" hm"<< ( tm2.tv_usec - tm.tv_usec ) <<std::endl;

    return 1;
}
