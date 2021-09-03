#include "log_tools.h"
#include <time.h>
#include <iostream>

#define REBUILDHOURS   7

using namespace std;

LogTools::LogTools()
{
  m_apiName = "";
  m_level   = 0;
}

LogTools::~LogTools()
{

}

LogTools::LogTools(const string &api_name, const int level)
{
  m_apiName = api_name;
  m_level   = level;
}

//创建日志
int LogTools::createLog(void)
{
  //生成日志类
  if(!initlog(m_apiName, m_level))
  {
      return -2;
  }
  //设置日志生成时间
  m_uiLastTime = getCurrTime();
  return 0;
}

//重建日志文件
void LogTools::rebuild(void)
{
    unsigned long long curr = getCurrTime();
    if((m_uiLastTime - curr) < 1) //无需生成日志
    {
        return ;
    }
    unsigned long long currHours = getCurrHours();
    if(currHours < REBUILDHOURS) //无需生成日志
    {
        return ;
    }	
    delete goDebugTrace;
    goDebugTrace = NULL;
    int proof = createLog();
    TRACE(1, "create log successful");
    if(proof < 0) //重建日志错误
    {
        TRACE(LOG_ERROR, "An error occurred while rebuild the log file! " << proof);
        return ;
    }

    m_uiLastTime = curr; //重置生成时间
}

//获取当前日期:年月日
unsigned long long LogTools::getCurrTime(void)
{
    time_t ti = time(NULL);
    struct tm *local = localtime(&ti);

    ostringstream buf;
    buf << local->tm_year + 1900
        << local->tm_mon + 1
        << local->tm_mday;

    return atoll(buf.str().c_str());
}

//获取当前时间 hours
unsigned long long LogTools::getCurrHours(void)
{
	time_t ti = time(NULL);
  struct tm *local = localtime(&ti);
  
  ostringstream buf;
  buf << local->tm_hour;
  
  return atoll(buf.str().c_str());
}

