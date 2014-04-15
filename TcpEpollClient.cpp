/* 
 * File:   TcpEpollClient.cpp
 * Author: gwm <wenmao@eastmoney.com>
 * 
 * Created on 2014年4月15日, 下午3:02
 */

#include "TcpEpollClient.h"
#include "Epoll.h"
#include "MemPool.h"

namespace CommLib {

    TcpEpollClientImp::TcpEpollClientImp(boost::shared_ptr<Epoll> epoll, boost::shared_ptr<MemPool> pMemPool)
    : epoll_(epoll),mempool_(pMemPool) {
    }

    TcpEpollClientImp::~TcpEpollClientImp() {
    }

    int TcpEpollClientImp::OnConnected() {
        epoll_->epollAdd(this, Epoll::EVENT_READ);
    }

    int TcpEpollClientImp::OnClose() {
        epoll_->epollDel(this);
    }

    int TcpEpollClientImp::OnRecv() {
        AllocPack* pack = mempool_->Alloc(1024);
        if (pack) {
            int len = Recv((void*) pack->getbuffer(), pack->getsize());

            if (len > 0) {
                pack->SetLength(len);
                OnRecvPack(pack);
            }
            pack->release();

            return len;
        }
        return -1;
    };

    int TcpEpollClientImp::OnSend() {
        return 1;
    };

}
