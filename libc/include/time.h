//
// Created by o0kam1 on 2025/05/12.
//

#ifndef TIME_H
#define TIME_H

#include <restrict.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef __UINT64_TYPE__ clock_t;
typedef __UINT64_TYPE__ time_t;

struct timespec {
    time_t tv_sec;
    long tv_nsec;
};

#ifndef __cplusplus
typedef struct timespec timespec;
#endif

struct tm {
    int tm_sec;
    int tm_min;
    int tm_hour;
    int tm_mday;
    int tm_mon;
    int tm_year;
    int tm_wday;
    int tm_yday;
    int tm_isdst;
};

#ifndef __cplusplus
typedef struct tm tm;
#endif

clock_t clock(void);
double difftime(time_t time1, time_t time0);
time_t mktime(tm* timeptr);
time_t timegm(tm* timeptr);
time_t time(time_t* timer);
int timespec_get(timespec* ts, int base);
int timespec_getres(timespec* ts, int base);
[[deprecated]] char* asctime(const tm* timeptr);
[[deprecated]] char* ctime(const time_t* timer);
tm* gmtime(const time_t* timer);
tm* gmtime_r(const time_t* timer, tm* buf);
tm* localtime(const time_t* timer);
tm* localtime_r(const time_t* timer, tm* buf);
size_t strftime(char* restrict s, size_t maxsize, const char* restrict format,
                const tm* restrict timeptr);

#ifdef __cplusplus
}
#endif

#endif //TIME_H
