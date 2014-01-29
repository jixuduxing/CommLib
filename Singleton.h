/* 
 * File:   Singleton.h
 * Author: Administrator
 *
 * Created on 2014年1月28日, 上午10:31
 */

#ifndef SINGLETON_H
#define	SINGLETON_H
namespace CommLib {

    template <class T>
    class Singleton : boost::noncopyable {
    public:

        static T& instance() {
            static T _instance;
            return _instance;
        };
    protected:

        Singleton(void) {
        };

        virtual ~Singleton(void) {
        };

    };
}
#endif	/* SINGLETON_H */

