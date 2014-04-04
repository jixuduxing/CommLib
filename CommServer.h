/* 
 * File:   CommonServer.h
 * Author: gwm <wenmao@eastmoney.com>
 *
 * Created on 2014年4月4日, 上午9:55
 */

#ifndef COMMONSERVER_H
#define	COMMONSERVER_H

#include "TcpEpollServer.h"

namespace CommLib {

    template<class ClientType>
    class CommonServer : public TcpEpollServerImp {
    public:

        CommonServer(
                boost::shared_ptr<Epoll> epoll,
                boost::shared_ptr<MemPool> mempool)
        : TcpEpollServerImp(epoll,"CommonServer","check",5), Mempool_(mempool) {
        }

        typedef std::list<boost::shared_ptr<TcpEpollSockImp> >::iterator pClientList;

    private:

        virtual boost::shared_ptr<TcpEpollSockImp> MakeNewClient(int socket) {
            return boost::shared_ptr<TcpEpollSockImp>(new ClientType(socket, Mempool_, this));
        }

        virtual void OnAddClient(boost::shared_ptr<TcpEpollSockImp> sock) {
            CAutoLock lock(lock_);
            ClientList_.push_back(sock);
        }

        virtual void OnCloseClient(boost::shared_ptr<TcpEpollSockImp> sock) {
            CAutoLock lock(lock_);
            std::list<boost::shared_ptr<TcpEpollSockImp> >::iterator iter = ClientList_.begin();
            for (; iter != ClientList_.end(); iter++) {
                if (sock == *iter)
                    ClientList_.erase(iter);
            }
        };

        void Check() {
            CAutoLock lock(lock_);
            std::list<boost::shared_ptr<TcpEpollSockImp> >::iterator iter = ClientList_.begin();
            for (; iter != ClientList_.end();) {
                if (!(*iter)->CheckValid()) {
                    RemoveClient((*iter));
                    ClientList_.erase(iter++);
                    continue;
                }
                iter++;
            }
        }

    private:

        boost::shared_ptr<MemPool> Mempool_;

        CMutexLock lock_;
        std::list<boost::shared_ptr<TcpEpollSockImp> > ClientList_;
    };

    class CommSockImp : public TcpEpollSockImp {
    public:

        CommSockImp(int sock, boost::shared_ptr<MemPool> pMemPool
                , TcpEpollServerImp* pEServ
                );

    private:
        virtual int OnRecvPack(AllocPack* pack) = 0;

        int OnRecv();

        void SetLastRecvTime();

        bool CheckValid();

    private:
        boost::shared_ptr<MemPool> pMemPool_;

        Time LastRecvTime_;
        TimeSpan TimeOut_;
    };
}
#endif	/* COMMONSERVER_H */

