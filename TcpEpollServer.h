/* 
 * File:   TcpEpoolServer.h
 * Author: Administrator
 *
 * Created on 2014年1月28日, 下午2:29
 */

#ifndef TCPEPOOLSERVER_H
#define	TCPEPOOLSERVER_H
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <list>

#include "Thread.h"
#include "MemPool.h"
#include "Epoll.h"
#include "TcpSock.h"

namespace CommLib {

    template<class ClientType>
    class TcpEpollServer :  public Thread, public TcpServSock {
    public:

        TcpEpollServer(
                boost::shared_ptr<Epoll> epoll,
                boost::shared_ptr<MemPool> mempool)
        : Epoll_(epoll), Mempool_(mempool), bStarted_(false),
        Thread("TcpEpollServer", boost::bind(&TcpEpollServer::Schedule, this)) {
        }

        virtual ~TcpEpollServer() {
        }

        typedef std::list<boost::shared_ptr<TcpSock> >::iterator pClientList;

        bool Start(int nPort) {
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

            assert ( 0 == Epoll_->epollAdd( this, Epoll::EVENT_READ ) );
            this->Setnonblocking();

            StartThread();
        }

        bool Stop() {

//            Epoll_->Close();
            if (!bStarted_)
                return false;
            bStarted_ = false;

            Join();
        }

        bool Schedule() {
            while (Epoll_->Schedule(10));
            return true;
        }

        //         

        virtual int OnAccept() {
            sockaddr_in addr;
            socklen_t addrlen = sizeof (sockaddr_in);
            int socket = accept(Getsock(), (struct sockaddr *) &addr, &addrlen);
            if (socket < 0) {
                return 0;
            }

            boost::shared_ptr<TcpSock> sock(MakeNewClient(socket));
            SockAddr skAddr( addr);

            sock->Setnonblocking();            
            assert ( 0 == Epoll_->epollAdd( sock.get(), Epoll::EVENT_READ ) );
            sock->SetRemoteAddr( skAddr );
            ClientList_.push_back(sock);

            return 1;
        }

        ClientType* MakeNewClient(int socket) {
            return new ClientType(socket);
        }

    private:
        bool bStarted_;
        boost::shared_ptr<Epoll> Epoll_;
        boost::shared_ptr<MemPool> Mempool_;

        std::list<boost::shared_ptr<TcpSock> > ClientList_;
    };
}

#endif	/* TCPEPOOLSERVER_H */

