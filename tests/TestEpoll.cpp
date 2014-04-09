/* 
 * File:   TestEpoll.cpp
 * Author: Administrator
 *
 * Created on 2014-1-28, 11:10:10
 */

#include <stdlib.h>
#include <iostream>

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "../TcpSock.h"
#include "../Parser.h"
#include "../Epoll.h"
#include "../MemPool.h"
#include "../CommServer.h"
#include "../Time.h"

/*
 * Simple C++ Test Suite
 */
class TestParser : public CommLib::ProtocolParser {
public:

    virtual bool parse(char* buff, int len) {
        char *buffnew = new char[ len + 1 ];
        memcpy(buffnew, buff, len);
        buffnew[ len ] = 0;

        std::cout << buffnew << std::endl;
    }
};

void test1() {
    std::cout << "TestEpoll test 1" << std::endl;

    //    CommLib::Epoll2 ep;
    //    TestParser* parser = new TestParser();
    //
    //    CommLib::SockAddr addr(2080, "0.0.0.0", CommLib::Sock::IP);
    //
    //    CommLib::TcpServSock ssock(parser, &ep);
    //    ssock.Bind(addr);
    //    ssock.Listen(10);
    //    ssock.RegistEpoll();
    //
    //    while (ep.Schedule(10)) {
    //    }

    std::cout << " end " << std::endl;
}


//在该类中做计时、超时处理

class MyTcpSock : public CommLib::CommSockImp {
public:

    MyTcpSock(int sock, boost::shared_ptr<CommLib::MemPool> pMemPool, CommLib::TcpEpollServerImp* pEServ) :
    CommLib::CommSockImp(sock, pMemPool, pEServ,15),
    pParser_(new TestParser()) {
    }

    virtual int OnSend() {
        return 1;
    };

    virtual int OnClose() {
        return 1;
    };

    virtual int OnRecvPack(CommLib::AllocPack* pack) {

        pParser_->parse((char*) pack->getbuffer(), pack->getlengh());
        Send((char*) pack->getbuffer(), pack->getlengh());

        return 1;
    }

private:
    boost::shared_ptr<TestParser> pParser_;
};

void test2() {

    CommLib::CommonServer<MyTcpSock> server(
            boost::shared_ptr<CommLib::Epoll >(new CommLib::EpollSimple()),
            boost::shared_ptr<CommLib::MemPool >(new CommLib::MemPool()));
    server.Start(2080);
    while (1) {
        sleep(1);
        //        server.Schedule();
    }

    std::cout << "TestEpoll test 2" << std::endl;
    //    std::cout << "%TEST_FAILED% time=0 testname=test2 (TestEpoll) message=error message sample" << std::endl;
}

int main(int argc, char** argv) {
    std::cout << "%SUITE_STARTING% TestEpoll" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% test1 (TestEpoll)" << std::endl;
    test1();
    std::cout << "%TEST_FINISHED% time=0 test1 (TestEpoll)" << std::endl;

    std::cout << "%TEST_STARTED% test2 (TestEpoll)\n" << std::endl;
    test2();
    std::cout << "%TEST_FINISHED% time=0 test2 (TestEpoll)" << std::endl;

    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}

