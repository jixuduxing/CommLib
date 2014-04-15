
#include "CommServer.h"

namespace CommLib {

    CommSockImp::CommSockImp(int sock, boost::shared_ptr<MemPool> pMemPool
            , TcpEpollServerImp* pEServ,
            int timeOut
            )
    : TcpEpollSockImp(sock, pEServ),
    TimeOut_(timeOut),
    pMemPool_(pMemPool) {
        LastRecvTime_ = Time::GetCurrentTime();
    }

    void CommSockImp::SetLastRecvTime() {
        LastRecvTime_ = Time::GetCurrentTime();
    };

    bool CommSockImp::CheckValid() {
        Time t = Time::GetCurrentTime();
        if (t - LastRecvTime_ > TimeOut_) {
            //超时
            printf("超时\r\n");
            Close();
            return false;
        }

        return true;
    }

    int CommSockImp::OnRecv() {
        SetLastRecvTime();
        AllocPack* pack = pMemPool_->Alloc(1024);
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
    }

}
