#include "common/json/json.h"
#include "common/define.h"

#include "common/curl_http.h"

#include <string>
#include <mysql/mysql.h>

using namespace std;

class WxInfo
{
public:
	WxInfo();
	~WxInfo();

	inline void cleanWxInfo()
	{
		userInfo_	= "";
		code_		= "";
		stampTime_	= "";
		sign_		= "";
	}

public:
	string		userInfo_;		//用户信息的json字符串
	string		code_;			//用户获取session_key的密钥
	string		stampTime_;		//时间戳
	string		sign_;			//签名
};

struct UserInfo
{
public:
	string		nickName_;		//用户昵称
	int			sex_;			//用户性别 0 未知, 1 男, 2 女
	string		photo_;			//用户头像url
	string		city_;			//用户所在城市
	string		openid_;		//获取的用户微信唯一标识
	string		unionid_;		//用户在开放平台的唯一标识符
	string		sessionKey_;	//用户的会话秘钥
};

class ChatRegister
{
public:
	ChatRegister();
	~ChatRegister();
	

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

	//解析userInfo的json串
	int			parseUserInfo(UserInfo &);
	//获取用户的session_key和openid
	int			getSessionKey(UserInfo &);

	//检验用户是否已经注册过
	int			checkUser(const UserInfo &);

	//把用户信息写入数据库
	int			insertMysql(const UserInfo &);

	//用户存在, 更新session_key
	int			updateMysql(const UserInfo &);

	//获取用户的uid
	int			getUserUid(Json::Value &, UserInfo &);

	//插入记录到在线状态表
	int			insertStatus();
	
	//清理 
	void		clean();

private:
	string		mip_;
	string		mport_;
	string		mdbName_;
	string		mdbUser_;
	string		mdbUserPwd_;

	string		wxSessionUrl_;
	string		appid_;
	string		appSecret_;

	MYSQL		*conn_;///连接数据库的句柄

	WxInfo		wxInfo_;
	int			userIdx_;

	string		callback_;
};

