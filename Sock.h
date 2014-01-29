/* 
 * File:   Sock.h
 * Author: Administrator
 *
 * Created on 2014年1月27日, 下午2:47
 */

#ifndef SOCK_H
#define	SOCK_H

#include <netinet/in.h>
#include <arpa/inet.h>

#include <string>
#include <boost/noncopyable.hpp>

namespace CommLib {

    class Sock;

    class SockAddr {
    public:
        SockAddr() {
        }

        SockAddr(short sin_port, std::string addr, int sin_family) {
            addr_.sin_port = htons(sin_port);
            addr_.sin_family = sin_family;
            addr_.sin_addr.s_addr = inet_addr(addr.c_str());
        }

        SockAddr( sockaddr_in& addr )
        {
            addr_ = addr;
        }
        
        sockaddr_in& GetAddr_in() {
            return addr_;
        }
        
        void SetAddr_in(sockaddr_in& addr) {
            addr_ = addr;
        }

    private:
        struct sockaddr_in addr_;
        friend class Sock;
    };

    class Sock : boost::noncopyable {
    public:

        enum {
            IP = PF_INET,
            TCP = SOCK_STREAM,
            UDP = SOCK_DGRAM,
        };

        Sock();
        Sock(int __domain, int __type, int __protocol);

        void Attach(int sock);
        int Bind(SockAddr& addr);
        int Connect(SockAddr& addr);
        int Listen(int nlistens);

        int SetLocalAddr(SockAddr& addr);
        SockAddr& GetLocalAddr();
        int SetRemoteAddr(SockAddr& addr);
        SockAddr& GetRemoteAddr();
        
        int Setnonblocking();
        int Getsock();
        void Close();

        virtual int Send(void *buf, int nbytes) = 0;
        virtual int Recv(void *buf, int nbytes) = 0;

        virtual int OnRecv() = 0;
        virtual int OnSend() = 0;
        virtual int OnClose() = 0;

        virtual ~Sock();
        
    private:
        int sock_;

        SockAddr localaddr_;
        SockAddr remoteaddr_;
    };

    
}
#endif	/* SOCK_H */

