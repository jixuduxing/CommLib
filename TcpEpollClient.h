/* 
 * File:   TcpEpollClient.h
 * Author: gwm <wenmao@eastmoney.com>
 *
 * Created on 2014年4月15日, 下午3:02
 */

#ifndef TCPEPOLLCLIENT_H
#define	TCPEPOLLCLIENT_H
#include <boost/shared_ptr.hpp>
#include "TcpSock.h"

namespace CommLib {

    class AllocPack;
    class MemPool;
    class Epoll;
    
    class TcpEpollClientImp:public TcpClientSock {
    public:
        TcpEpollClientImp(boost::shared_ptr<Epoll> epoll, boost::shared_ptr<MemPool> pMemPool);
        virtual ~TcpEpollClientImp();

    public:
        virtual int OnRecvPack(AllocPack* pack) = 0;

    private:

        virtual int OnConnected();
        virtual int OnClose();
        virtual int OnRecv();
        virtual int OnSend();


        boost::shared_ptr<Epoll> epoll_;
        boost::shared_ptr<MemPool> mempool_;
    };
}
#endif	/* TCPEPOLLCLIENT_H */

