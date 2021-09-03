 //***************************************************************
 // @file:    onlineThread.h
 // @author:  dingfang
 // @date    2018-10-25 17:51:43
 //***************************************************************
 
/*
 * 统计在线人数线程
 * */

#define __ONLINETHREAD_H__
#define __ONLINETHREAD_H__

#include "../common/define.h"
#include "../common/thread.h"

#include <vector>

class OnlineThread
: Thread
{
public:
	OnlineThread(map< int, set<int> > &userMapSet)
	: userMapSet_(userMapSet)
	{	}
	~OnlineThread()	{	}

	void run();

private:
	/*
	 * 函数说明: 更新数据库用户在线状态
	 * */
	bool updateOnlineStatus();

private:
	MYSQL		*conn_;
	SqlMysql	sqlMysql_;
	map< int, set<int> > &userMapSet_;    //存储: room_id <-> list<fd>
	std::vector<int> roomIdVec_;
};

#endif
