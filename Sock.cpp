/* 
 * File:   Sock.cpp
 * Author: Administrator
 * 
 * Created on 2014年1月27日, 下午2:47
 */

#include <fcntl.h>
#include <sys/socket.h>

#include "Sock.h"


namespace CommLib {

    Sock::Sock(int __domain, int __type, int __protocol) {
        sock_ = socket(__domain, __type, __protocol);
    }

    int Sock::Bind(SockAddr& addr) {
        localaddr_ = addr;
        return bind(sock_, (struct sockaddr*) &addr.GetAddr_in(), sizeof (addr));
    }

    int Sock::Connect(SockAddr& addr) {
        remoteaddr_ = addr;
        return connect(sock_, (struct sockaddr*) &addr.GetAddr_in(), sizeof (addr));
    }

    int Sock::Listen(int nlistens) {
        return listen(sock_, nlistens);
    }

    int Sock::Setnonblocking() {
        int ctl = fcntl(sock_, F_GETFL);
        return fcntl(sock_, F_SETFL, ctl | O_NONBLOCK);
    }

    Sock::~Sock() {
    }

    void Sock::Attach(int sock) {
        sock_ = sock;
    }

    int Sock::SetLocalAddr(SockAddr& addr) {
        localaddr_ = addr;
    }

    SockAddr& Sock::GetLocalAddr() {
        return localaddr_;
    }

    int Sock::SetRemoteAddr(SockAddr& addr) {
        remoteaddr_ = addr;
    }

    SockAddr& Sock::GetRemoteAddr() {
        return remoteaddr_;
    }

    int Sock::Getsock() {
        return sock_;
    }

    void Sock::Close() {
        close(sock_);
    }

}

