# include "common/json/json.h"

# include <string>
# include <vector>
# include <algorithm>
# include <utility>
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

class BbDetails
{
public:
	BbDetails();
	~BbDetails();
	

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

	//获取简介
	int			getSummary(Json::Value &);

	//获取简介附加数据
	int			getSummaryAdditional(Json::Value &);

	//获取原型防御的数据
	int			getProtoTypeDefenses();

	//获取详细数据
	int			getBbDetails();

	//获取详细数据附加数据
	int			getBbDetailsAdditional();
	
	//把数据打包到一块儿
	int			pack(Json::Value &);

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

	vector< pair<int, Json::Value> >		dataVec_;
};

