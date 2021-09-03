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
	}

public:
	int			userIdx_;		//用户uid
	string		stampTime_;		//时间戳
	string		sign_;			//签名
};

class ChatHome
{
public:
	ChatHome();
	~ChatHome();
	
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
	int			getHomePage(Json::Value &);

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
