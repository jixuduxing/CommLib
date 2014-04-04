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
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>

#include "Thread.h"
#include "MemPool.h"
#include "Epoll.h"
#include "TcpSock.h"
#include "Time.h"

namespace CommLib {

    //    template<class ClientType>

    class TcpEpollServerImp;

    class TcpEpollSockImp : public TcpSock, public boost::enable_shared_from_this<TcpEpollSockImp> {
    public:

        TcpEpollSockImp(int sock
                , TcpEpollServerImp* pEServ
                );


    public:
        bool CheckValid();

        inline void SetLastRecvTime() {
            LastRecvTime_ = Time::GetCurrentTime();
        };
    private:

        int OnSend();
        int OnClose();

    private:
        Time LastRecvTime_;
        TimeSpan TimeOut_;
        TcpEpollServerImp* pEServ_;
    };

    class TcpEpollServerImp : public Thread, public TcpServImp {
    public:

        TcpEpollServerImp(
                boost::shared_ptr<Epoll> epoll);

        virtual ~TcpEpollServerImp() {
        }

    public:
        bool Start(int nPort);
        bool Stop();

        //         
    private:
        int OnAccept();
        int OnSend();
        int OnClose();

        bool Schedule();
        void RemoveClient(boost::shared_ptr<TcpEpollSockImp> sock);
        
    private:
        virtual boost::shared_ptr<TcpEpollSockImp> MakeNewClient(int socket) = 0;

        virtual void OnAddClient(boost::shared_ptr<TcpEpollSockImp> sock) = 0;

        virtual void OnCloseClient(boost::shared_ptr<TcpEpollSockImp> sock) = 0;

    private:
        bool bStarted_;
        boost::shared_ptr<Epoll> Epoll_;

    };

}

#endif	/* TCPEPOOLSERVER_H */

