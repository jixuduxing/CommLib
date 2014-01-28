/* 
 * File:   TcpEpoolServer.cpp
 * Author: Administrator
 * 
 * Created on 2014年1月28日, 下午2:29
 */

#include "TcpEpollServer.h"
#include "Sock.h"
namespace CommLib {

    TcpEpollServer::TcpEpollServer(
            boost::shared_ptr<Epoll> epoll,
            boost::shared_ptr<MemPool> mempool)
    : Epoll_(epoll), Mempool_(mempool),bStarted_(false),
    Thread("TcpEpollServer", boost::bind(&TcpEpollServer::Schedule, this)) {
    }

    TcpEpollServer::~TcpEpollServer() {
    }

    bool TcpEpollServer::Schedule() {
        while (Epoll_->Schedule(10));
    }

    bool TcpEpollServer::Start(int nPort) {
        if( bStarted_ )
            return false;
        bStarted_ = true;
        
        SockAddr addr(nPort, "0.0.0.0", Sock::IP);

        if( -1 == Bind(addr) )
        {
            perror("TcpEpollServer::Start false");
            return false;
        }
        
        if( -1 == Listen(10 ) )
        {
            perror("TcpEpollServer::Start false");
            return false;
        }
        
        StartThread();
    }

    int TcpEpollServer::OnAccept() {
        sockaddr_in addr;
        socklen_t addrlen = sizeof (sockaddr_in);
        int socket = accept(Getsock(), (struct sockaddr *) &addr,&addrlen);
        if (socket < 0) {
            delete sock;
            return 0;
        }
        
        boost::shared_ptr<TcpSock> sock( new ClientType(socket) );
        sock->SetRemoteAddr(addr);
        ClientList_.push_back( sock );
    }

    bool TcpEpollServer::Stop() {
        
        if( !bStarted_ )
            return false;
        bStarted_ = false;
        
        Join();
    }
}