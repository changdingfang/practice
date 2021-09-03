#include "timeTools.h"
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <sstream>
#include <iostream>
#include <string.h>

using namespace std;

//获取当前毫秒
long long TimeTools::getMsec(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);

    return tv.tv_sec * 1000 + tv.tv_usec / 100;
}

//获取当前日期:年月日
unsigned long long TimeTools::getCurrTime(void)
{
    time_t ti = time(NULL);
    struct tm *local = localtime(&ti);

    ostringstream buf;
    buf << local->tm_year + 1900
        << local->tm_mon + 1
        << local->tm_mday;

    return atoll(buf.str().c_str());
}


//获取当前时间:年月日
string TimeTools::getCurrTime(const char *mark, const char *colon /*=NULL*/)
{
    time_t ti = time(NULL);
    struct tm *local = localtime(&ti);
    ostringstream buf;

    if(strlen(mark) <= 0)
    {
        buf << local->tm_year + 1900
            << local->tm_mon + 1
            << local->tm_mday
            << local->tm_hour
            << local->tm_min
            << local->tm_sec;
    }
    else
    {
        buf << local->tm_year + 1900
            << mark
            << local->tm_mon + 1
            << mark
            << local->tm_mday;

        if(colon == NULL)
        {
            buf << " " << local->tm_hour
                << ":"
                << local->tm_min
                << ":"
                << local->tm_sec;
        }
        else
        {
            buf << local->tm_hour
                << colon
                << local->tm_min
                << colon
                << local->tm_sec;
        }
    }

    return buf.str();
}






