/* 
 * File:   TcpEpoolServer.cpp
 * Author: Administrator
 * 
 * Created on 2014年1月28日, 下午2:29
 */

#include "TcpEpollServer.h"

namespace CommLib {

    TcpEpollSockImp::TcpEpollSockImp(int sock
            , TcpEpollServerImp* pEServ
            )
    : TcpSock(sock)
    , pEServ_(pEServ) {

    }

    int TcpEpollSockImp::OnSend() {
        return 0;
    }

    int TcpEpollSockImp::OnClose() {
        Close();

        pEServ_->OnCloseClient(shared_from_this());
        return 0;
    }

    void CheckLoop::LoopTask() {
        pImp->Check();
    }

    TcpEpollServerImp::TcpEpollServerImp(
            boost::shared_ptr<Epoll> epoll,
            std::string thrname,
            std::string checklpname, int looptime)
    : Epoll_(epoll), bStarted_(false),
    Thread(boost::bind(&TcpEpollServerImp::Schedule, this), thrname),
    checkloop_(checklpname, looptime) {
        checkloop_.pImp = this;
    }

    bool TcpEpollServerImp::Start(int nPort) {
        if (bStarted_)
            return false;
        bStarted_ = true;

        SockAddr addr(nPort, "0.0.0.0", Sock::IP);

        if (-1 == Bind(addr)) {
            perror("TcpEpollServer::Start false");
            return false;
        }

        if (-1 == Listen(10)) {
            perror("TcpEpollServer::Start false");
            return false;
        }

        assert(0 == Epoll_->epollAdd(this, Epoll::EVENT_READ));
        this->Setnonblocking();

        StartThread();

        checkloop_.StartThread();
    }

    bool TcpEpollServerImp::Stop() {

        //            Epoll_->Close();
        if (!bStarted_)
            return false;
        bStarted_ = false;

        Join();

        checkloop_.StopLoop();
        checkloop_.Join();
    }

    bool TcpEpollServerImp::Schedule() {
        while (Epoll_->Schedule(10));
        return true;
    }

    //         

    int TcpEpollServerImp::OnAccept() {
        sockaddr_in addr;
        socklen_t addrlen = sizeof (sockaddr_in);
        int socket = accept(Getsock(), (struct sockaddr *) &addr, &addrlen);
        if (socket < 0) {
            return 0;
        }

        boost::shared_ptr<TcpEpollSockImp> sock = MakeNewClient(socket);
        SockAddr skAddr(addr);

        sock->Setnonblocking();
        assert(0 == Epoll_->epollAdd(sock.get(), Epoll::EVENT_READ));
        sock->SetRemoteAddr(skAddr);

        OnAddClient(sock);
        return 1;
    }

    int TcpEpollServerImp::OnSend() {
        return 1;
    };

    int TcpEpollServerImp::OnClose() {
        Close();
        return Stop();
    };

    void TcpEpollServerImp::RemoveClient(boost::shared_ptr<TcpEpollSockImp> sock) {
        Epoll_->epollDel(sock.get());
    }

}
