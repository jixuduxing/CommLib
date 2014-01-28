/* 
 * File:   TimeSpan.h
 * Author: Administrator
 *
 * Created on 2014年1月27日, 上午8:51
 */

#ifndef TIMESPAN_H
#define	TIMESPAN_H

#include <time.h>
namespace CommLib {

    class TimeSpan {
    public:
        TimeSpan();
        TimeSpan(time_t tt);
        TimeSpan(const TimeSpan& orig);
        TimeSpan(long lDays, int nHours, int nMins, int nSecs);
        virtual ~TimeSpan();

        long GetDays();
        int GetHours();
        int GetMinutes();
        int GetSeconds();
        time_t GetTimeSpan();

        TimeSpan operator +(TimeSpan &span);
        TimeSpan operator -(TimeSpan &span);
        bool operator>(TimeSpan &span);
        
    private:
        long lDays_;
        int nHours_;
        int nMins_;
        int nSecs_;
        time_t tt_;
    };
}
#endif	/* CTIMESPAN_H */

