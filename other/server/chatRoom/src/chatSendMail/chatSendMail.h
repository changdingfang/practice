#include "common/json/json.h"
#include "common/define.h"

#include "common/curl_http.h"

#include <unistd.h>
#include <string>
#include <mysql/mysql.h>

using namespace std;

class Info
{
public:
	Info() {};
	~Info() {};

	inline void cleanInfo()
	{
		mail_		= "";
		randNum_	= "";
		stampTime_	= "";
		sign_		= "";
	}

public:
	string		mail_;			//邮箱等
	string		randNum_;		//随机验证码
	string		stampTime_;		//时间戳
	string		sign_;			//签名
};

class SendMail
{
public:
	SendMail();
	~SendMail();
	

	//读取配置
	void		readDBConfig();
	//处理
	int 		messageHandle(const string &, string &);

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

	//检测邮箱是否已经注册
	int			checkMail();

	//把随机码写入数据库
	int			insertMysql();

	//把随机验证码发送给客户端
	int			sendMail();

	//清理 
	void		clean();

private:
	string		mip_;
	string		mport_;
	string		mdbName_;
	string		mdbUser_;
	string		mdbUserPwd_;

	string		pythonPath_;
	string		pythonFile_;

	MYSQL		*conn_;///连接数据库的句柄

	Info		info_;
};

