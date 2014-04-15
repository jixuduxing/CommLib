/* 
 * File:   TestTcpEpollClient.cpp
 * Author: gwm <wenmao@eastmoney.com>
 *
 * Created on 2014-4-15, 15:24:58
 */

#include <stdlib.h>
#include <iostream>
#include "TcpEpollClient.h"
#include "Epoll.h"
#include "MemPool.h"

class myClient : public CommLib::TcpEpollClientImp {
public:

    myClient(boost::shared_ptr<CommLib::Epoll> epoll, boost::shared_ptr<CommLib::MemPool> pMemPool)
    : CommLib::TcpEpollClientImp(epoll,pMemPool) {
    };

    virtual int OnRecvPack(CommLib::AllocPack* pack) {

        int len = pack->getlengh();
        char *buffnew = new char[ len + 1 ];
        memcpy(buffnew, pack->getbuffer(), len);
        buffnew[ len ] = 0;

        std::cout << buffnew << std::endl;
//        pParser_->parse((char*) pack->getbuffer(), pack->getlengh());
        Send((char*) pack->getbuffer(), pack->getlengh());

        return 1;
    }
    
   
};

/*
 * Simple C++ Test Suite
 */

void test1() {
    std::cout << "TestTcpEpollClient test 1" << std::endl;
}

void test2() {
    std::cout << "TestTcpEpollClient test 2" << std::endl;
    std::cout << "%TEST_FAILED% time=0 testname=test2 (TestTcpEpollClient) message=error message sample" << std::endl;
}

int main(int argc, char** argv) {
    
    boost::shared_ptr<CommLib::Epoll > ep(new CommLib::EpollSimple());
    myClient mm( ep,boost::shared_ptr<CommLib::MemPool >(new CommLib::MemPool()) );
    mm.InitConnect("127.0.0.1",2080);
    mm.Send((void*)"sdsd",4);
    while(ep->Schedule(10))
        sleep(2);
    
//    std::cout << "%SUITE_STARTING% TestTcpEpollClient" << std::endl;
//    std::cout << "%SUITE_STARTED%" << std::endl;
//
//    std::cout << "%TEST_STARTED% test1 (TestTcpEpollClient)" << std::endl;
//    test1();
//    std::cout << "%TEST_FINISHED% time=0 test1 (TestTcpEpollClient)" << std::endl;
//
//    std::cout << "%TEST_STARTED% test2 (TestTcpEpollClient)\n" << std::endl;
//    test2();
//    std::cout << "%TEST_FINISHED% time=0 test2 (TestTcpEpollClient)" << std::endl;
//
//    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}

