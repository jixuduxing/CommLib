/* 
 * File:   Epoll.h
 * Author: Administrator
 *
 * Created on 2014年1月27日, 下午2:13
 */

#ifndef EPOLL_H
#define	EPOLL_H

#include <sys/epoll.h>
#include <boost/noncopyable.hpp>

#define Epoll_MaxSize 1024

namespace CommLib {

    class Sock;

    class Epoll : boost::noncopyable {
    public:

        enum EpollEvent {
            EVENT_READ = EPOLLIN,
            EVENT_WRITE = EPOLLOUT,
            EVENT_CLOSE = EPOLLHUP,
            EVENT_LET = EPOLLET, //边缘触发  默认水平触发
        };

        enum EpollOp {
            Op_ADD = EPOLL_CTL_ADD,
            Op_DEL = EPOLL_CTL_DEL,
            Op_MOD = EPOLL_CTL_MOD,
        };

        Epoll(int size = Epoll_MaxSize);
        virtual ~Epoll();

        virtual bool Schedule(int sec) = 0;
        int epollAdd(Sock* sock, int event);
        int epollDel(Sock* sock);
        int epollMod(Sock* sock, int event);

        virtual int epollwait(struct epoll_event *events, int maxevents, int timeout);

        int GetEpollSize()
        {
            return epollsize_;
        }
    protected:
        struct epoll_event* pEvents_;

    private:
        int epoll_;
        int epollsize_;
        int epollctl(EpollOp op, Sock* sock, int event);
    };

    class EpollSimple : public Epoll {
    public:

        EpollSimple(int size = Epoll_MaxSize)
        : Epoll(size) {
        };
        ~EpollSimple();
        virtual bool Schedule(int sec);
    };
}
#endif	/* EPOOL_H */

