# include "common/json/json.h"

#include "common/define.h"

# include <string>
# include <algorithm>
# include <utility>
# include <mysql/mysql.h>

using namespace std;

class RequestInfo
{
public:
	RequestInfo();
	~RequestInfo();

	inline void cleanRequestInfo()
	{
		userIdx_	= 0;
		stampTime_	= "";
		sign_		= "";
		pageNum_	= 0;
		page_		= 0;
		roomId_		= 0;
	}

public:
	int			userIdx_;		//用户uid
	string		stampTime_;		//时间戳
	string		sign_;			//签名
	int			pageNum_;		//一页的数量
	int			page_;			//页码
	int			roomId_;		//房间id
};

class OnlineUser
{
public:
	OnlineUser();
	~OnlineUser();
	
	//读取配置
	void		readDBConfig();

	//处理
	int 		messageHandle(const string &, string &, string &);

	//初始化数据库
	int 		initDB();

private:
	//重连数据库
	int 		reConnectDB();

	//解析读取的配置数据
	string		trim(string &);
	
	//解析参数
	int			parseQuery(const string &);
	void 		getParameter(const string &, const string &, string &);

	//获取首页信息
	int			getOnlineUser(Json::Value &);

	//清理 
	void		clean();

private:
	string		mip_;
	string		mport_;
	string		mdbName_;
	string		mdbUser_;
	string		mdbUserPwd_;

	MYSQL		*conn_;///连接数据库的句柄

	RequestInfo			requestInfo_;	//请求参数存储的结构体
	string				callback_;
	map<int, string>	roomNameMap_;	//房间号<->房间名字
};
