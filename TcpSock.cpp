/* 
 * File:   TcpSock.cpp
 * Author: Administrator
 * 
 * Created on 2014年1月28日, 上午10:17
 */

#include "TcpSock.h"
#include "Epoll.h"
#include "MemPool.h"
#include "Parser.h"

namespace CommLib {

    int TcpSock::Send(void *buf, int nbytes) {
        return send(sock_, buf, nbytes, 0);
    }

    int TcpSock::Recv(void *buf, int nbytes) {
        return recv(sock_, buf, nbytes, 0);
    }

    int TcpSock::OnRecv() {
        AllocPack* pack = MemPool::defaultMp()->Alloc(1024);
        if (pack) {
            int len = Recv((void*) pack->getbuffer(), pack->getsize());

            if (len > 0) {
                pack->SetLength(len);
                pParser_->parse((char*) pack->getbuffer(), pack->getlengh());
            }
            pack->release();

            return len;
        }
        return -1;
    }

    int TcpSock::RegistEpoll() {
        return pPoll_->epollAdd( this, Epoll::EVENT_READ );
    }

    int TcpSock::UnRegistEpoll() {
        return pPoll_->epollDel( this );
    }

    int TcpSock::OnSend() {
        return 0;
    }

    int TcpSock::OnClose()
    {
        UnRegistEpoll();
        return 1;
    }
    
    int TcpServSock::OnRecv() {
        TcpSock* sock = new TcpSock(pParser_, pPoll_);
        socklen_t addrlen = sizeof (sockaddr_in);
        int socket = accept(Getsock(), (struct sockaddr *) &(sock->GetRemoteAddr().GetAddr_in()),
                &addrlen);
        if (socket < 0) {
            delete sock;
            return 0;
        }
        sock->Attach(socket);
        if ( sock->RegistEpoll() < 0 ) {
            sock->Close();
            delete sock;
            return 0;
        }
        return -1;
    }
}
