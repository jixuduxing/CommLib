
#include "CommServer.h"

namespace CommLib {

    
    CommSockImp::CommSockImp(int sock, boost::shared_ptr<MemPool> pMemPool
            , TcpEpollServerImp* pEServ
            )
    : TcpEpollSockImp(sock, pEServ),
    pMemPool_(pMemPool) {
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
