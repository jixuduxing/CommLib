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

#include "ThreadLoop.h"
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
        virtual bool CheckValid() = 0;

    private:
        int OnSend();
        int OnClose();

    private:

        TcpEpollServerImp* pEServ_;
    };

    class CheckLoop : public ThreadLoop {
    public:

        CheckLoop(std::string name = "", int loopTime = 3)
        : ThreadLoop(name, loopTime) {
        }

        virtual void LoopTask();
        TcpEpollServerImp* pImp;
    };

    class TcpEpollServerImp : public Thread, public TcpServImp {
    public:


        TcpEpollServerImp(
                boost::shared_ptr<Epoll> epoll,
                std::string thrname = "TcpEpollServerImp",
                std::string checklpname = "", int looptime = 3);

        virtual ~TcpEpollServerImp() {
        }

    public:
        bool Start(int nPort);
        bool Stop();

        virtual void Check() = 0;
        
        void OnCloseClient(boost::shared_ptr<TcpEpollSockImp> sock);
        //         
    private:
        int OnAccept();
        int OnSend();
        int OnClose();


        bool Schedule();
        

    private:
        virtual boost::shared_ptr<TcpEpollSockImp> MakeNewClient(int socket) = 0;

        virtual void OnAddClient(boost::shared_ptr<TcpEpollSockImp> sock) = 0;

    public:
        virtual void OnClientClose(boost::shared_ptr<TcpEpollSockImp> sock) = 0;

    private:
        bool bStarted_;
        boost::shared_ptr<Epoll> epoll_;

        CheckLoop checkloop_;
    };

}

#endif	/* TCPEPOOLSERVER_H */

