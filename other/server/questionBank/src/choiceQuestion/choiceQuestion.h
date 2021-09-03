# include "common/json/json.h"

# include <string>
# include <mysql/mysql.h>

using namespace std;

class UserInfo
{
public:
	UserInfo();
	~UserInfo();

	inline void cleanUserInfo()
	{
		page_		= 0;
		pageNum_	= 0;
	}

public:
	int		page_;
	int		pageNum_;
};

class ChoiceQuestion
{
public:
	ChoiceQuestion();
	~ChoiceQuestion();
	

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

	//获取题干
	int			getStem();

	//获取答案
	int			getAnswer();
	
	//获取选项
	int			getOption();
	
	//整合
	int			pageJson(Json::Value&);

	//清理 
	void		clean();

private:
	string		mip_;
	string		mport_;
	string		mdbName_;
	string		mdbUser_;
	string		mdbUserPwd_;

	MYSQL		*conn_;///连接数据库的句柄

	UserInfo	userInfo_;

	map<int, Json::Value>	stemMap_;

	string		listStemId_;

	string		callback_;
};

