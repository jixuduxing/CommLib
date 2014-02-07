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

    //include the ptr point to the next
    class AllocPack2 : public AllocPack {
    protected:
        AllocPack2* next_;
        friend class SimpleList<AllocPack2>;
        friend class AllocPackListSimple;
        
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
    
    //using SimpleList Writen by self
    class AllocPackListSimple : public AllocPackList {
    public:
        AllocPackListSimple(int ind);
        virtual ~AllocPackListSimple();

        virtual void PrintfSelf(std::ostream& os);
        
        virtual AllocPack* Alloc( );
        
        virtual void Free(AllocPack* pPack);
    private:
        SimpleList<AllocPack2> PackList;
    };

    class MemPool : boost::noncopyable {
    public:
        enum MemPoolType
        {
            default_Mp,         //using deque
            careusing_Mp,    //care memory using
            SimpleList_Mp,    //using the list self
        };
        
        static MemPool* defaultMp( )
        {
            static MemPool* defmp = new MemPool();
            return defmp;
        }
        
        //default 0,no care the mem using,else care
        MemPool(MemPoolType type = default_Mp);
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

