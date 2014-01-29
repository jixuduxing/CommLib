/* 
 * File:   CTime.h
 * Author: Administrator
 *
 * Created on 2014年1月26日, 下午4:49
 */

#ifndef TIME_H
#define	TIME_H

#include "TimeSpan.h"
namespace CommLib {

    class Time {
    public:
        Time(time_t tt);
        Time();
        Time(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec);
        Time(const Time& orig);
        virtual ~Time();

        int GetDay();
        int GetDayOfWeek();
        int GetHour();
        int GetMinute();
        int GetMonth();
        int GetSecond();
        time_t GetTime();
        int GetYear();

        static Time GetCurrentTime();
        static long GetCurrTick();

        Time operator +(TimeSpan &span);
        Time operator +(time_t tt);
        Time operator -(time_t tt);

        Time operator -(TimeSpan &span);
        TimeSpan operator -(Time &tm);
    private:
        tm tm_;
        time_t tt_;
    };
}
#endif	/* CTIME_H */

