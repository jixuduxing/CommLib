/* 
 * File:   MemPool.h
 * Author: Administrator
 *
 * Created on 2014年1月22日, 下午1:56
 */

#ifndef MEMPOOL_H
#define	MEMPOOL_H

#include <math.h>
#include <vector>
#include <pthread.h>
#include <iostream>
#include <list>
#include <deque>

#include "Lock.h"

namespace CommLib {
    class AllocPackList;

    class AllocPack : boost::noncopyable {
    public:
        const char* getbuffer();

        int getlengh();
        int getsize();

        void reset();
        void release();

        void SetLength(int len);
    protected:
        friend class AllocPackList;
        friend class AllocPackList2;

        AllocPackList* pAllocList_;
        AllocPack(int size, AllocPackList* palloc);
        ~AllocPack();

    private:
        int len_;
        char *buff_;
        int size_;

        bool Append(char* buff, int len);

        template<typename T>
        bool Append(T n);
    };

    //
    // Min Size:7 Bytes
#define MIN_SIZE 3
    //
    // Max Size:32M -1
#define MAX_SIZE 25

    class MemPool;

    class AllocPackList : boost::noncopyable {
    public:
        AllocPackList(int ind);
        virtual ~AllocPackList();

        virtual AllocPack* Alloc();
        virtual void Free(AllocPack* pPack);
        virtual void PrintfSelf(std::ostream& os);

        int Size() {
            return buffsize_;
        }

    protected:
        int buffsize_;

    protected:
        CMutexLock lock_;
        std::deque<AllocPack*> PackFreeList_;
    };

    //care the mem using

    class AllocPackList2 : public AllocPackList {
    public:
        AllocPackList2(int ind);
        virtual ~AllocPackList2();

        virtual AllocPack* Alloc();
        virtual void Free(AllocPack* pPack);
        virtual void PrintfSelf(std::ostream& os);
    private:
        std::list<AllocPack*> PackUsingList_;

    };

    class MemPool : boost::noncopyable {
    public:
        static MemPool* defaultMp()
        {
            static MemPool* defmp = new MemPool();
            return defmp;
        }
        
        //default 0,no care the mem using,else care
        MemPool(int type = 0);
        virtual ~MemPool();

        AllocPack* Alloc(int size);
        void Free(AllocPack* pPack);

        void PrintfSelf(std::ostream& os);
    private:
        std::vector<AllocPackList*> AllocPackListVec_;
        int VectSize_;

    private:

        int hash(int n);
        int hashbit(int n);
    };
}
#endif	/* MEMPOOL_H */

