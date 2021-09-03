#include "CgiInterface.h"
#include <string>
#include <stdlib.h>

CgiInterface::CgiInterface(void)
{
}

CgiInterface::~CgiInterface(void)
{
}

//获取参数
bool CgiInterface::getParameter(const std::string &key, const std::string &query, std::string &reply)
{
    size_t valuePos = query.find(key.c_str());
    if(std::string::npos == valuePos)
    {
        reply.clear(); //没有找到对应的参数
        return false;
    }
    size_t markPos = query.find_first_of("&", valuePos);

    valuePos += key.length();
    reply = query.substr(valuePos, markPos - valuePos);

    return true;
}

bool CgiInterface::getParameter(const std::string &key, const std::string &query, uint64_t &reply)
{
	std::string buf;
	if(!getParameter(key, query, buf))
	{
		return false;
	}

	reply = atoll(buf.c_str());
	return true;
}
bool CgiInterface::getParameter(const std::string &key, const std::string &query, int &reply)
{
	std::string buf;
	if(!getParameter(key, query, buf))
	{
		return false;
	}
	
	reply = atoi(buf.c_str());
	return true;
}

bool CgiInterface::getParameter(const std::string &key, const std::string &query, double &reply)
{
	std::string buf;
	if(!getParameter(key, query, buf))
	{
		return false;
	}

	reply = atof(buf.c_str());
	return true;
}
