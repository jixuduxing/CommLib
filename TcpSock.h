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
    class ProtocolParser;
    class Epoll;

    class TcpSock : public Sock {
    public:

        TcpSock(ProtocolParser* pParser, Epoll*pPoll)
        : Sock(PF_INET, SOCK_STREAM, 0)
        , pParser_(pParser), pPoll_(pPoll) {
        }

        ~TcpSock() {
        }

    public:
        int RegistEpoll();
        int UnRegistEpoll();

        virtual int Send(void *buf, int nbytes);
        virtual int Recv(void *buf, int nbytes);

        virtual int OnRecv();
        virtual int OnSend();
        virtual int OnClose();

    protected:
        ProtocolParser* pParser_;
        Epoll* pPoll_;
    };

    class TcpServSock : public TcpSock {
    public:
        TcpServSock(ProtocolParser* pParser, Epoll*pPoll)
        : TcpSock(pParser, pPoll) {
        }
    public:
        virtual int OnRecv();
        
    };
}
#endif	/* TCPSOCK_H */

