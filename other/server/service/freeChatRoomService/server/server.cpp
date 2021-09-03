 //***************************************************************
 // @file:    server.cpp
 // @author:  dingfang
 // @date    2018-10-24 17:50:29
 //***************************************************************
 
#include "../common/epoll.h"
#include "../common/log_tools.h"

using namespace std;

int main(void)
{
	LogTools log_tools("charRoom", 5);
	if (0 != log_tools.createLog())
	{
		return -1;
	}
	TRACE(1, "char room start");
	
	Epoll ep;
	ep.loop();
	return 0;
}
