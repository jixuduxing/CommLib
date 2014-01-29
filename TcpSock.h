/* 
 * File:   TcpSock.h
 * Author: Administrator
 *
 * Created on 2014年1月28日, 上午10:17
 */

#ifndef TCPSOCK_H
#define	TCPSOCK_H

#include "Sock.h"

namespace CommLib {
    //    class ProtocolParser;
    //    class Epoll;

    class TcpSock : public Sock {
    public:

        TcpSock();

        TcpSock(int sock) ;

        ~TcpSock() ;

    public:
        virtual int Send(void *buf, int nbytes);
        virtual int Recv(void *buf, int nbytes);
    };

    class TcpServSock : public TcpSock {
    public:

    public:
        virtual int OnRecv();
        virtual int OnSend();
        virtual int OnClose();
        
        virtual int OnAccept() = 0;

    };
}
#endif	/* TCPSOCK_H */

