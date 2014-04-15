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

        TcpSock(int sock);

        ~TcpSock();

    public:
        virtual int Send(void *buf, int nbytes);
        virtual int Recv(void *buf, int nbytes);
    };

    class TcpServImp : public TcpSock {
    public:

    public:
        int OnRecv();

        virtual int OnAccept() = 0;



    };

    class TcpClientSock : public TcpSock {
    public:
        TcpClientSock();
    public:
        bool InitConnect(char* szIp, int nPort);
        virtual int OnConnected() = 0;
        //        virtual int OnRecv() = 0;
        //        virtual int OnSend() = 0;
        //        virtual int OnClose() = 0;

    private:
        bool bConnected_;
    };
}
#endif	/* TCPSOCK_H */

