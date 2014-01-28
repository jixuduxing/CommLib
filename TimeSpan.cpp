/* 
 * File:   TimeSpan.cpp
 * Author: Administrator
 * 
 * Created on 2014年1月27日, 上午8:51
 */

#include "TimeSpan.h"
namespace CommLib {

    TimeSpan::TimeSpan() : lDays_(0), nHours_(0), nMins_(0), nSecs_(0), tt_(0) {
    }

    TimeSpan::TimeSpan(time_t tt) {
        tt_ = tt;
        nSecs_ = tt % 60;
        tt = tt / 60;

        nMins_ = tt % 60;
        tt = tt / 60;

        nHours_ = tt % 24;
        lDays_ = tt / 24;
    }

    TimeSpan::TimeSpan(const TimeSpan& orig)
    : lDays_(orig.lDays_), nHours_(orig.nHours_), nMins_(orig.nMins_), nSecs_(orig.nSecs_)
    , tt_(orig.tt_) {
    }

    TimeSpan::~TimeSpan() {
    }

    TimeSpan::TimeSpan(long lDays, int nHours, int nMins, int nSecs)
    : lDays_(lDays), nHours_(nHours), nMins_(nMins), nSecs_(nSecs) {
        tt_ = ((lDays_ * 24 + nHours_)*60 + nMins_)*60 + nSecs_;
    }

    long TimeSpan::GetDays() {
        return lDays_;
    }

    int TimeSpan::GetHours() {
        return nHours_;
    }

    int TimeSpan::GetMinutes() {
        return nMins_;
    }

    int TimeSpan::GetSeconds() {
        return nSecs_;
    }

    time_t TimeSpan::GetTimeSpan() {
        return tt_;
    }

    TimeSpan TimeSpan::operator +(TimeSpan &span) {
        TimeSpan rSpan(tt_ + span.tt_);
        return rSpan;
    }

    TimeSpan TimeSpan::operator -(TimeSpan &span) {
        TimeSpan rSpan(tt_ - span.tt_);
        return rSpan;
    }

    bool TimeSpan::operator>(TimeSpan &span) {
        return (tt_ > span.tt_);
    }
}
