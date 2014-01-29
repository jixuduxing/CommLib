/* 
 * File:   Epoll.cpp
 * Author: Administrator
 * 
 * Created on 2014年1月27日, 下午2:13
 */

#include "Epoll.h"
#include "Sock.h"
#include <assert.h>

namespace CommLib {

    Epoll::Epoll(int size) {
        assert(size <= Epoll_MaxSize);
        epollsize_ = size;
        pEvents_ = new struct epoll_event[epollsize_];
        epoll_ = epoll_create(epollsize_);
    }

    Epoll::~Epoll() {
        close(epoll_);
    }

    int Epoll::epollctl(EpollOp op, Sock* sock, int event) {
        struct epoll_event ev;
        ev.events = event;
        ev.data.fd = sock->Getsock();
        ev.data.ptr = sock;
        return epoll_ctl(epoll_, op, sock->Getsock(), &ev);
    }

    int Epoll::epollwait(struct epoll_event *events, int maxevents, int timeout) {
        return epoll_wait(epoll_, events, maxevents, timeout);
    }

    int Epoll::epollAdd(Sock* sock, int event) {
        return epollctl(Op_ADD, sock, event);
    }

    int Epoll::epollDel(Sock* sock) {
        return epollctl(Op_DEL, sock, 0);
    }

    int Epoll::epollMod(Sock* sock, int event) {
        return epollctl(Op_MOD, sock, event);
    }

    EpollSimple::EpollSimple(int size)
    : Epoll(size) {
    }

    EpollSimple::~EpollSimple() {
    }

    bool EpollSimple::Schedule(int sec) {

        int nfds = epollwait(pEvents_, GetEpollSize(), sec);
        if (-1 != nfds) {
            for (int i = 0; i < nfds; i++) {
                Sock* sk = (Sock*) pEvents_[i].data.ptr;
                if (pEvents_[i].events & Epoll::EVENT_CLOSE) {
                    sk->OnClose();
                    this->epollDel(sk);
                } else {
                    if (pEvents_[i].events & Epoll::EVENT_READ)
                        sk->OnRecv();

                    if (pEvents_[i].events & Epoll::EVENT_WRITE)
                        sk->OnSend();
                }
            }
            return true;
        }
        
        if ( errno == EINTR )  
        {
           return true;  
        }
        
        perror("EpollSimple::Schedule");
        return false;
    }
}
