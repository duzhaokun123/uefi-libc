//
// Created by o0kam1 on 2025/05/14.
//

#include <time.h>

#include "cstdio"
#include "uefi.h"

clock_t clock() {
    return -1; // TODO:
}

double difftime(time_t time1, time_t time0) {
    return time1 - time0;
}

time_t mktime(tm* timeptr);

time_t timegm(tm* timeptr);

time_t time(time_t* timer) {
    auto efiTime = EFI_TIME();
    RT->GetTime(&efiTime, nullptr);
    // TODO: convert EFI_TIME to time_t, wtf why no unix timestamp in EFI_TIME
    time_t t = -1;
    if (timer != nullptr) {
        *timer = t;
    }
    return t;
}

int timespec_get(timespec* ts, int base);

int timespec_getres(timespec* ts, int base);

const char* wwwMap[] = {
    "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
};

const char* mmmMap[] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

char ctimeBuffer[26];

[[deprecated]]
char* asctime(const tm* timeptr) {
    auto www = wwwMap[timeptr->tm_wday];
    auto mmm = mmmMap[timeptr->tm_mon];
    auto dd = timeptr->tm_mday;
    auto hh = timeptr->tm_hour;
    auto mm = timeptr->tm_min;
    auto ss = timeptr->tm_sec;
    auto yyyy = timeptr->tm_year + 1900;
    sprintf(ctimeBuffer, "%3s %3s %02d %02d:%02d:%02d %04d\n", www, mmm, dd, hh, mm, ss, yyyy);
    return ctimeBuffer;
}

[[deprecated]]
char* ctime(const time_t* timer) {
    auto t = localtime(timer);
    auto r = asctime(t);
    return r;
}

tm* gmtime(const time_t* timer);

tm* gmtime_r(const time_t* timer, tm* buf);

tm* localtime(const time_t* timer);

tm* localtime_r(const time_t* timer, tm* buf);

size_t strftime(char* restrict s, size_t maxsize, const char* restrict format, const tm* restrict timeptr);
