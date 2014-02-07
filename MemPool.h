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

        virtual void reset();
        void release();

        void SetLength(int len);
    protected:
        friend class AllocPackList;
        friend class AllocPackList2;

        AllocPackList* pAllocList_;
        AllocPack(int size, AllocPackList* palloc);
        ~AllocPack();

    protected:
        int len_;
        char *buff_;
        int size_;

        bool Append(char* buff, int len);

        template<typename T>
        bool Append(T n);
    };

    class AllocPack2 : public AllocPack {
    public:
        AllocPack2* next_;
        friend class AllocPackList3;
    protected:
        virtual void reset()
        {
            next_ = NULL;
            AllocPack::reset();
        }
        AllocPack2(int size, AllocPackList* palloc)
                :AllocPack( size,palloc)
        {
            next_ = NULL;
        }
        ~AllocPack2();
    };
    
    template<typename T>
    class SimpleList
    {
        
    public:
        SimpleList()
        {
            head_ = NULL;
            tail_ = NULL;
            size_ = 0;
        }
        
        ~SimpleList()
        {
        }
        
        void AddTail(T* node)
        {
            if( tail_ )
            {
                tail_->next_ = node;
                tail_ = node;
            }
            else
                head_ = tail_ = node;
            
            size_ ++;
        }
        
        T* RemoveHead()
        {
            T* temp = head_;
            if( temp )
            {
                head_ = temp->next_;
                size_ --;
                if( 0 == size_ )
                {
                    tail_ = NULL;
                }
            }
            
            return temp;
        }
        
        int size()
        {
            return size_;
        }
    private:
        T* head_;
        T* tail_;
        int size_;
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
    
    class AllocPackList3 : public AllocPackList {
    public:
        AllocPackList3(int ind);
        virtual ~AllocPackList3();

        virtual void PrintfSelf(std::ostream& os)
        {
            CAutoLock al(lock_);
            os << " free:" << PackList.size() << std::endl;
        }
        virtual AllocPack* Alloc( )
        {
            CAutoLock al(lock_);
            AllocPack* pPack = NULL;
            pPack = PackList.RemoveHead();
            if( !pPack )
                pPack = new AllocPack2( buffsize_,this );
//            if( NULL != pPackList )
//            {
//                pPack = pPackList;
//                pPackList = pPackList->next_;
//            }
//            else
//            {
//                pPack = new AllocPack2( buffsize_,this );
//            }
            
            return pPack;
        }
        
        virtual void Free(AllocPack* pPack)
        {
            CAutoLock al(lock_);

            pPack->reset();
            PackList.AddTail( (AllocPack2*)pPack );
            
//            AllocPack2* temp = NULL;
//            
//            if( pPackList )
//            {
//                temp = pPackList;
//                while( temp->next_ )
//                {
//                    temp = temp->next_;
//                }
//                temp->next_ =  (AllocPack2*)pPack;
//            }
//            else
//                pPackList =  (AllocPack2*)pPack;
            
//            pPackList->next_ = (AllocPack2*)pPack;
        }
    private:
        AllocPack2* pPackList;
        
        SimpleList<AllocPack2> PackList;
    };

    class MemPool : boost::noncopyable {
    public:
        static MemPool* defaultMp( )
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

