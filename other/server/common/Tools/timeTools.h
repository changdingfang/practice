#ifndef TIMETOOLS_H__
#define TIMETOOLS_H__
#include <string>

class TimeTools
{
public:
    //获取当前毫秒
    static long long getMsec(void);

    //获取当前时间:年月日
    static unsigned long long getCurrTime(void);

    //获取当前时间:年月日
    static std::string getCurrTime(const char *mark, const char *colon = NULL);
};

#endif //TIMETOOLS_H__
