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
#include "../TcpEpollServer.h"
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

class MyTcpEpollServer;
//在该类中做计时、超时处理

class MyTcpSock : public CommLib::TcpSock, public boost::enable_shared_from_this<MyTcpSock> {
public:

    MyTcpSock(int sock, boost::shared_ptr<CommLib::MemPool> pMemPool, MyTcpEpollServer* pEServ) :
    CommLib::TcpSock(sock),
    pMemPool_(pMemPool),
    pParser_(new TestParser()),
    pEServ_(pEServ) {
    }

    //    MyTcpSock()
    //    {
    //        pParser_ = new TestParser();
    //    }

    virtual int OnRecv() {
        using namespace CommLib;

        LastRecvTime_ = CommLib::Time::GetCurrentTime();
        AllocPack* pack = pMemPool_->Alloc(1024);
        if (pack) {
            int len = Recv((void*) pack->getbuffer(), pack->getsize());

            if (len > 0) {
                pack->SetLength(len);
                pParser_->parse((char*) pack->getbuffer(), pack->getlengh());
            }
            this->Send( (char*) pack->getbuffer(), pack->getlengh() );
            pack->release();

            return len;
        }
        return -1;
    }

    virtual int OnSend() {
        return 0;
    }

    virtual int OnClose();

    bool CheckValid() {
        if (CommLib::Time::GetCurrentTime() - LastRecvTime_ > TimeOut_) {
            //超时
            printf("超时\r\n");
            Close();
            return false;
        }
        
        return true;
    }

private:
    CommLib::Time LastRecvTime_;
    CommLib::TimeSpan TimeOut_;

    boost::shared_ptr<CommLib::MemPool> pMemPool_;
    boost::shared_ptr<TestParser> pParser_;

    MyTcpEpollServer* pEServ_;
};

class MyTcpEpollServer : public CommLib::TcpEpollServer {
public:

    MyTcpEpollServer(
            boost::shared_ptr<CommLib::Epoll> epoll,
            boost::shared_ptr<CommLib::MemPool> mempool)
    : CommLib::TcpEpollServer(epoll), Mempool_(mempool) {
    }

    virtual boost::shared_ptr<TcpSock> MakeNewClient(int socket) {
        return boost::shared_ptr<TcpSock>( new MyTcpSock(socket,Mempool_,this) );
    }

    virtual void OnAddClient(boost::shared_ptr<TcpSock> sock) {
        CommLib::CAutoLock lock(lock_);
        ClientList_.push_back(sock);
    };

    virtual void OnCloseClient(boost::shared_ptr<TcpSock> sock) {
        CommLib::CAutoLock lock(lock_);
        std::list<boost::shared_ptr<TcpSock> >::iterator iter = ClientList_.begin();
        for (; iter != ClientList_.end(); iter++) {
            if (sock == *iter)
                ClientList_.erase(iter);
        }
    };

    void Check()
    {
        CommLib::CAutoLock lock(lock_);
         std::list<boost::shared_ptr<TcpSock> >::iterator iter = ClientList_.begin();
        for (; iter != ClientList_.end(); ) {
            boost::shared_ptr<MyTcpSock> psock = boost::dynamic_pointer_cast<MyTcpSock>( *iter);
            if( psock )
            {
                if( !psock->CheckValid() )
                {
                    RemoveClient(psock);
                    ClientList_.erase(iter++ );
                    continue;
                }
            }
            iter++;
        }
    }
    
    boost::shared_ptr<CommLib::MemPool> Mempool_;

    CommLib::CMutexLock lock_;
    std::list<boost::shared_ptr<TcpSock> > ClientList_;
};

int MyTcpSock::OnClose()
 {
        Close();

        pEServ_->OnCloseClient(shared_from_this());
        return 0;
    }

void test2() {

    MyTcpEpollServer server(
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

