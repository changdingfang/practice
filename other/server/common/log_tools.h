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
  
  //�ؽ���־�ļ�
  void rebuild(void);
  
  //������־�ļ�
  int createLog(void);
  
  //��ȡ��ǰʱ��:������ʱ��
  unsigned long long getCurrTime(void);
  
  //��ȡ��ǰʱ�� hours
  unsigned long long getCurrHours(void);
  
private:
    int                 m_level;        //��־��¼����
    unsigned long long  m_uiLastTime;   //��һ�θ���ʱ��
    std::string         m_apiName;      //��������
};

#endif //LOGTOOLS_H__
