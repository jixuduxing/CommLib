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

namespace CommLib {

    class Sock;
    class Epoll : boost::noncopyable {
    public:

        enum EpollEvent {
            EVENT_READ = EPOLLIN,
            EVENT_WRITE = EPOLLOUT,
            EVENT_LET = EPOLLET,        //边缘触发  默认水平触发
        };

        enum EpollOp {
            Op_ADD = EPOLL_CTL_ADD,
            Op_DEL = EPOLL_CTL_DEL,
            Op_MOD = EPOLL_CTL_MOD,
        };

        Epoll();
        virtual ~Epoll();

        int epollAdd( Sock* sock, int event ); 
        int epollDel( Sock* sock );
        int epollMod( Sock* sock, int event );

        virtual int epollwait( struct epoll_event *events, int maxevents, int timeout );
    private:
        int epoll_;
        int epollctl( EpollOp op, Sock* sock, int event );
    };
    
#define Epoll_Size 1024
    
    class Epoll2:public Epoll
    {
    public:
        void Schedule( int sec );
        struct epoll_event events[Epoll_Size];
    };
}
#endif	/* EPOOL_H */

