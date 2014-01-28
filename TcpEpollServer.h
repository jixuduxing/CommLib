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
    class TcpEpollServer : boost::noncopyable,Thread,TcpServSock {
    public:
        TcpEpollServer( 
                boost::shared_ptr<Epoll> epoll,
                boost::shared_ptr<MemPool> mempool );
        virtual ~TcpEpollServer( );

        typedef std::list<boost::shared_ptr<TcpSock> >::iterator pClientList;

        bool Start( int nPort );
        bool Stop( );

        bool Schedule();
        
        virtual int OnAccept();
    private:
        bool bStarted_;
        boost::shared_ptr<Epoll> Epoll_;
        boost::shared_ptr<MemPool> Mempool_;

        std::list<boost::shared_ptr<TcpSock> > ClientList_;
    };
}

#endif	/* TCPEPOOLSERVER_H */

