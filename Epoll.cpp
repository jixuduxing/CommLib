/* 
 * File:   Epoll.cpp
 * Author: Administrator
 * 
 * Created on 2014年1月27日, 下午2:13
 */

#include "Epoll.h"
#include "Sock.h"

namespace CommLib {

    Epoll::Epoll() {
        epoll_ = epoll_create(10);
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

    int Epoll::epollDel(Sock* sock ) {
        return epollctl(Op_DEL, sock, 0);
    }

    int Epoll::epollMod(Sock* sock, int event) {
        return epollctl(Op_MOD, sock, event);
    }

    void Epoll2::Schedule(int sec) {
        int nfds = epollwait( events, Epoll_Size, sec );
        if (-1 != nfds) {
            for (int i = 0; i < nfds; i++) {
                Sock* sk = (Sock*) events[i].data.ptr;
                sk->OnRecv();
            }
        }
    }
}
