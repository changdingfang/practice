# include "common/json/json.h"

# include <string>
# include <mysql/mysql.h>

using namespace std;

class BbInfo
{
public:
	BbInfo();
	~BbInfo();

	inline void cleanBbInfo()
	{
		type_		= 0;
		stampTime_	= "";
		sign_		= "";
	}

public:
	int			type_;			//标签类型
	string		stampTime_;		//时间戳
	string		sign_;			//签名
};

class BbHomePage
{
public:
	BbHomePage();
	~BbHomePage();
	

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

	//获取一级或者二级的信息
	int			getBbInfo(Json::Value &);
	
	//清理 
	void		clean();

private:
	string		mip_;
	string		mport_;
	string		mdbName_;
	string		mdbUser_;
	string		mdbUserPwd_;

	MYSQL		*conn_;///连接数据库的句柄

	BbInfo		bbInfo_;

	string		callback_;
};

