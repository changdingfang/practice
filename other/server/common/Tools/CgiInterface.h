#ifndef __CGIINTERFACE_H__
#define __CGIINTERFACE_H__
#include <string>
#include <stdint.h>

class CgiInterface
{
public:
	CgiInterface(void);
	virtual ~CgiInterface(void);
	//处理请求
	virtual int processRequest(const std::string request) = 0;

	//获取返回
	virtual std::string getReply(void) = 0;
protected:
	//阅读配置文件
	virtual int readConfig(const std::string &file) = 0;

	//获取参数
	bool getParameter(const std::string &key, const std::string &query, std::string &reply);

	bool getParameter(const std::string &key, const std::string &query, int &reply);

	bool getParameter(const std::string &key, const std::string &query, uint64_t &reply);

	bool getParameter(const std::string &key, const std::string &query, double &reply);
};

#endif //__CGIINTERFACE_H__
