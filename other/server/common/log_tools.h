#ifndef LOGTOOLS_H__
#define LOGTOOLS_H__

#include "debugtrace.h"

#include <string>

class LogTools
{
public:
	LogTools();
	~LogTools();
	
  LogTools(const std::string &aip_name, const int level);
  
  //重建日志文件
  void rebuild(void);
  
  //创建日志文件
  int createLog(void);
  
  //获取当前时间:年月日时分
  unsigned long long getCurrTime(void);
  
  //获取当前时间 hours
  unsigned long long getCurrHours(void);
  
private:
    int                 m_level;        //日志记录级别
    unsigned long long  m_uiLastTime;   //上一次更新时间
    std::string         m_apiName;      //程序名字
};

#endif //LOGTOOLS_H__
