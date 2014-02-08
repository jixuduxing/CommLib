/* 
 * File:   MemPool.cpp
 * Author: Administrator
 * 
 * Created on 2014年1月22日, 下午1:56
 */

#include <iostream>
#include <list>
#include <pthread.h>
#include "Lock.h"
#include "MemPool.h"
namespace CommLib {

    AllocPack::AllocPack(int size, AllocPackList* palloc) {
        buff_ = new char[size];
        size_ = size;
        len_ = 0;
        pAllocList_ = palloc;
    }

    AllocPack::~AllocPack() {
        delete buff_;
    }

    const char* AllocPack::getbuffer() {
        return buff_;
    }

    int AllocPack::getlengh() {
        return len_;
    }

    void AllocPack::SetLength(int len) {
        len_ = len;
    }

    int AllocPack::getsize() {
        return size_;
    }

    void AllocPack::reset() {
        len_ = 0;
    }

    void AllocPack::release() {
        pAllocList_->Free(this);
    }

    bool AllocPack::Append(char* buff, int len) {
        int len2 = len_ + len;
        if (len2 > size_)
            return false;

        memcpy(buff_ + len_, buff, len);
        len_ = len2;
        return 0;
    }

    template<typename T>
    bool AllocPack::Append(T n) {
        int len2 = len_ + sizeof (T);
        if (len2 > size_)
            return false;

        memcpy(buff_ + len_, &n, sizeof (T));
        len_ = len2;
        return 0;
    }

    AllocPackList::AllocPackList(int ind) {
        buffsize_ = (1 << (ind + MIN_SIZE)) - 1;
    }

    AllocPackList::~AllocPackList() {
        for (std::deque<AllocPack*>::iterator iter = PackFreeList_.begin();
                iter != PackFreeList_.end(); iter++) {
            delete (*iter);
        }
        PackFreeList_.clear();
    }

    AllocPack* AllocPackList::Alloc() {
        CAutoLock al(lock_);
        AllocPack* pPack = NULL;

        if (!PackFreeList_.empty()) {
            pPack = PackFreeList_.front();
            PackFreeList_.pop_front();
        } else {
            pPack = new AllocPack(buffsize_, this);
        }

        return pPack;
    }

    void AllocPackList::Free(AllocPack* pPack) {
        CAutoLock al(lock_);

        pPack->reset();
        PackFreeList_.push_back(pPack);
    }

    void AllocPackList::PrintfSelf(std::ostream& os) {
        CAutoLock al(lock_);
        os << " free:" << PackFreeList_.size() << std::endl;
    }

    AllocPackList2::AllocPackList2(int ind) : AllocPackList(ind) {
    }

    AllocPackList2::~AllocPackList2() {
        for (std::list<AllocPack*>::iterator iter = PackUsingList_.begin();
                iter != PackUsingList_.end(); iter++) {
            delete (*iter);
        }
        PackUsingList_.clear();
    }

    AllocPack* AllocPackList2::Alloc() {
        CAutoLock al(lock_);
        AllocPack* pPack = NULL;

        if (!PackFreeList_.empty()) {
            pPack = PackFreeList_.front();
            PackFreeList_.pop_front();
            PackUsingList_.push_back(pPack);
        } else {
            pPack = new AllocPack(buffsize_, this);
            PackUsingList_.push_back(pPack);
        }

        return pPack;
    }

    void AllocPackList2::Free(AllocPack* pPack) {
        CAutoLock al(lock_);

        pPack->reset();

        PackUsingList_.remove(pPack);
        PackFreeList_.push_back(pPack);
    }

    void AllocPackList2::PrintfSelf(std::ostream& os) {
        CAutoLock al(lock_);
        os << " using:" << PackUsingList_.size();
        os << " free:" << PackFreeList_.size() << std::endl;
    }

    AllocPackListSimple::AllocPackListSimple(int ind)
    : AllocPackList(ind)
    //    ,testpool( (1 << (ind + MIN_SIZE)) - 1 )
    {
    }

    AllocPackListSimple::~AllocPackListSimple() {
        for (AllocPack2* pPack = PackList.RemoveHead();
                pPack != NULL; pPack = PackList.RemoveHead()) {
            delete pPack;
        }
    }

    void AllocPackListSimple::PrintfSelf(std::ostream& os) {
        CAutoLock al(lock_);
        os << " free:" << PackList.size() << std::endl;
    }

    AllocPack* AllocPackListSimple::Alloc() {
        AllocPack* pPack = NULL;
        //        {
        //            CAutoLock al(lock_);
        //            pPack = ( AllocPack* )testpool.malloc();
        //            return pPack;
        //        }
        {
            CAutoLock al(lock_);
            pPack = PackList.RemoveHead();
        }

        if (!pPack)
            pPack = new AllocPack2(buffsize_, this);
        return pPack;
    }

    void AllocPackListSimple::Free(AllocPack* pPack) {
        pPack->reset();

        CAutoLock al(lock_);
        //        testpool.free((void*)pPack );
        //        return;
        PackList.AddTail((AllocPack2*) pPack);
    }

    AllocPackListBoost::AllocPackListBoost(int ind)
    : AllocPackList(ind), boostPackpool_((1 << (ind + MIN_SIZE)) - 1 + sizeof (AllocPack)) {
    }

    AllocPackListBoost::~AllocPackListBoost() {
    }

    void AllocPackListBoost::PrintfSelf(std::ostream& os) {
        CAutoLock al(lock_);
        //        os << " free:" << testpool. << std::endl;
    }

    AllocPack* AllocPackListBoost::Alloc() {
        AllocPack* pPack = NULL;
        CAutoLock al(lock_);
        pPack = (AllocPack*) boostPackpool_.malloc();
        pPack->size_ = buffsize_;
        pPack->len_ = 0;
        pPack->pAllocList_  = this;
        pPack->buff_ = ( (char*)pPack + sizeof(AllocPack) );

        return pPack;
    }

    void AllocPackListBoost::Free(AllocPack* pPack) {
        CAutoLock al(lock_);
        boostPackpool_.free((void*) pPack);
        return;
    }

    MemPool::MemPool(MemPoolType type) {
        VectSize_ = MAX_SIZE - MIN_SIZE + 1;

        AllocPackListVec_.resize(VectSize_);

        for (int i = 0; i < VectSize_; i++) {
            if (default_Mp == type)
                AllocPackListVec_[i] = new AllocPackList(i);
            else if (careusing_Mp == type)
                AllocPackListVec_[i] = new AllocPackList2(i);
            else if (Boost_Mp == type)
                AllocPackListVec_[i] = new AllocPackListBoost(i);
            else
                AllocPackListVec_[i] = new AllocPackListSimple(i);
        }
    }

    MemPool::~MemPool() {

        for (int i = 0; i < VectSize_; i++) {
            delete AllocPackListVec_[i];
        }

        AllocPackListVec_.clear();
    }

    AllocPack* MemPool::Alloc(int size) {
        AllocPack* pPack = NULL;

        int ind = hashbit(size);
        if (-1 == ind)
            return pPack;

        return AllocPackListVec_[ind]->Alloc();
    }

    void MemPool::Free(AllocPack* pPack) {
        int ind = hashbit(pPack->getsize());
        if (-1 == ind)
            return;

        AllocPackListVec_[ind]->Free(pPack);
    }

    void MemPool::PrintfSelf(std::ostream& os) {
        for (int i = 0; i < VectSize_; i++) {
            os << "ind:" << i;
            AllocPackListVec_[i]->PrintfSelf(os);
        }
    }

    int bits(int value) //how many bits
    {
        int x = 0;
        while (value) {
            value >>= 1;
            x++;
        }
        return x;
    }

    int MemPool::hashbit(int n) {
        if (0 == n)
            return -1;

        int nbits = bits(n);

        if (nbits <= MIN_SIZE)
            return 0;
        else if (nbits > MAX_SIZE)
            return -1;
        else
            return ( nbits - MIN_SIZE);
    }

    int MemPool::hash(int n) {
        if (0 == n)
            return -1;

        if (n > AllocPackListVec_[VectSize_ - 1]->Size()) {
            return -1;
        }

        int i = 0;
        for (; i < VectSize_; i++) {
            if (n > AllocPackListVec_[i]->Size()) {
            } else
                break;
        }

        return i;
    }
}
