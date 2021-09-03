/*
 * 作者：刘洋
 * 日期：2016-11-15
 * 描述：对异常操作封装
 * 版本：1.0.0.0
 * 说明：高版本可以取代低版本，反之不可以。1.0.0.0 > 没有版本号
 * */
#ifndef LOGTOOLS_H__
#define LOGTOOLS_H__

#include "../debugtrace.h"
#include "errorTools.h"
#include "fileTools.h"
#include "timeTools.h"

#include <string>

class LogTools
{
public:
    LogTools(const std::string &aip_name, const std::string &conf_path);

    //重建日志文件
    void rebuild(void);

    //创建日志文件
    int createLog(void);
private:
    int                 m_level;        //日志记录级别
    unsigned long long  m_uiLastTime;   //上一次更新时间
    std::string         m_apiName;      //程序名字
    std::string         m_confPath;     //配置文件连接
    std::string         m_path;         //日志存放路径
    ErrorTools          m_erroTool;     //错误处理

};

#endif //LOGTOOLS_H__
