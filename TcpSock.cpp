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

    TcpSock::TcpSock()
    : Sock(PF_INET, SOCK_STREAM, 0) {
    }

    TcpSock::TcpSock(int sock) {
        this->Attach(sock);
    }

    TcpSock::~TcpSock() {
    }

    int TcpSock::Send(void *buf, int nbytes) {
        return send(Getsock(), buf, nbytes, 0);
    }

    int TcpSock::Recv(void *buf, int nbytes) {
        return recv(Getsock(), buf, nbytes, 0);
    }

    //    int TcpSock::OnRecv() {
    //        AllocPack* pack = MemPool::defaultMp()->Alloc(1024);
    //        if (pack) {
    //            int len = Recv((void*) pack->getbuffer(), pack->getsize());
    //
    //            if (len > 0) {
    //                pack->SetLength(len);
    ////                pParser_->parse((char*) pack->getbuffer(), pack->getlengh());
    //            }
    //            pack->release();
    //
    //            return len;
    //        }
    //        return -1;
    //    }
    //
    //    int TcpSock::RegistEpoll() {
    //        return pPoll_->epollAdd(this, Epoll::EVENT_READ);
    //    }
    //
    //    int TcpSock::UnRegistEpoll() {
    //        return pPoll_->epollDel(this);
    //    }

    //    int TcpSock::OnSend() {
    //        return 0;
    //    }
    //
    //    int TcpSock::OnClose() {
    //  //      perror("OnClose");
    ////        UnRegistEpoll();
    //        Close();
    //        return 1;
    //    }

    int TcpServSock::OnRecv() {
        return OnAccept();
    }

    int TcpServSock::OnSend() {
        return 0;
    }

    int TcpServSock::OnClose() {
        Close();
        return 0;
    }
}
