# include "common/json/json.h"


# include <openssl/sha.h>

# include <string>
# include <mysql/mysql.h>

using namespace std;

class UserInfo
{
public:
	UserInfo();
	~UserInfo();

public:
	string	signature_;
	string	timestamp_;
	string	nonce_;
	string	echostr_;
	string	token_;
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

	int			sh1();

	//清理 
	void		clean();

	//SHA1加密
	bool SHA1(std::string in, std::string& out);
	string getFormattedText(unsigned char buf[], int len);

private:
	string		mip_;
	string		mport_;
	string		mdbName_;
	string		mdbUser_;
	string		mdbUserPwd_;

	MYSQL		*conn_;///连接数据库的句柄

	string			mySign_;

	vector<string>	vecToSign_;

	UserInfo	userInfo_;

	string		callback_;
};

