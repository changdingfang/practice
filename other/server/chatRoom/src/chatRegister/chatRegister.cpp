#include "chatRegister.h"
#include "common/log_tools.h"
#include "common/HttpClient.h"

#include <time.h>

#include <iostream>
#include <fstream>

WxInfo::WxInfo()
{
}

WxInfo::~WxInfo()
{

}

ChatRegister::ChatRegister() : mip_(""), mport_("")
							   , mdbName_(""), mdbUser_("")
							   , mdbUserPwd_("") 
							   , conn_(NULL)
{

}

ChatRegister::~ChatRegister()
{
	if (NULL != conn_)
	{
		mysql_close(conn_);
		conn_ = NULL;
	}
}

int ChatRegister::messageHandle(const string &request, string &response, string &callback)
{
	clean();

	int ret = parseQuery(request);
	if (0 != ret)
	{
		TRACE(LOG_ERROR, "parseQuery error, ret: " << ret << ", request: " << request);
		return -1;
	}
	callback = callback_;

	UserInfo userInfo;
	ret = parseUserInfo(userInfo);
	if (0 != ret)
	{
		TRACE(LOG_ERROR, "ChatRegister::messageHandle error, ret: " << ret << ", request: " << request);
		return -4;
	}
	ret = getSessionKey(userInfo);
	if (0 != ret)
	{
		TRACE(LOG_ERROR, "ChatRegister::messageHandle error, ret: " << ret << ", request: " << request);
		return -4;
	}

	ret = checkUser(userInfo);
	if (0 == ret)
	{//表示用户没有注册过, 首次使用小程序
		ret = insertMysql(userInfo);
		if (0 != ret)
		{
			TRACE(LOG_ERROR, "ChatRegister::messageHandle error, ret: " << ret << ", request: " << request);
			TRACE(LOG_ERROR, "insert mysql again......");
			ret = insertMysql(userInfo);
			if (0 != ret)
			{
				TRACE(LOG_ERROR, "ChatRegister::messageHandle error, ret: " << ret << ", request: " << request);
				return -10;
			}
		}
	}
	else if (1 == ret)
	{ //表示用户已经存在//以前有注册过 //更新session_key
		ret = updateMysql(userInfo);
		if (0 != ret)
		{
				TRACE(LOG_ERROR, "ChatRegister::messageHandle error, ret: " << ret << ", request: " << request);
				return -4;
		}
	}
	else
	{
		TRACE(LOG_ERROR, "ChatRegister::messageHandle error, ret: " << ret << ", request: " << request);
		return -4;
	}
	Json::Value jsonData;
	if (0 == ret)
	{
		ret = getUserUid(jsonData, userInfo);
		if (0 != ret)
		{
				TRACE(LOG_ERROR, "ChatRegister::messageHandle error, ret: " << ret << ", request: " << request);
				return -4;
		}
		ret = insertStatus();
		if (0 != ret)
		{
				TRACE(LOG_ERROR, "ChatRegister::messageHandle error, ret: " << ret << ", request: " << request);
				return -4;
		}
	}

	Json::Value jsonValue;
	jsonValue["code"]	= 0;
	jsonValue["msg"]	= "success";
	//jsonValue["info"]	= jsonData;
	jsonValue["user_idx"]= userIdx_;
	Json::FastWriter fastWriter;
	response = fastWriter.write(jsonValue);

	return 0;
}

string ChatRegister::trim(string& s)
{
	int n = 0;
	for (n = s.size() - 1; n >= 0; n--)
	{
		if (s[n] != ' ' && s[n] != '\t' && s[n] != '\n' && s[n] != '\r')
		{   
			break;    
		}   
	}

	return s.substr(0, n+1);
}

void ChatRegister::readDBConfig()
{
	ifstream is_file("conf.conf");
	if (!is_file.is_open())
	{
		TRACE(LOG_ERROR,  "Cannot find config: http_mobile_meet.conf" );
		return;
	}

	std::string line;
	while( std::getline(is_file, line) )
	{
		std::istringstream is_line(line);
		std::string key;
		if( std::getline(is_line, key, '=') )
		{
			std::string value;
			if( std::getline(is_line, value) )
			{
				if (key == "dbserver_ip")
				{
					this->mip_ = trim(value);
				}
				else if (key == "dbserver_port")
				{
					this->mport_ = trim(value);
				}
				else if (key == "db_chatname")
				{
					this->mdbName_ = trim(value);
				}
				else if (key == "db_user")
				{
					this->mdbUser_ = trim(value);
				}
				else if (key == "db_pwd")
				{
					this->mdbUserPwd_ = trim(value);
				}
				else if (key == "wx_chat_appid")
				{
					this->appid_ = trim(value);
				}
				else if (key == "wx_chat_appsecret")
				{
					this->appSecret_ = trim(value);
				}
				else if (key == "wx_session_url")
				{
					this->wxSessionUrl_ = trim(value);
				}
			}
		}
	}
}

int ChatRegister::initDB()
{
	//初始化
	conn_ = mysql_init(NULL);

	//连接数据库 
	if(!mysql_real_connect(conn_, mip_.c_str(), mdbUser_.c_str(), mdbUserPwd_.c_str(), mdbName_.c_str(), 0, NULL, 0))
	{
		TRACE(LOG_ERROR, "Error connecting to database :" << mysql_error(conn_));
	}
	else
	{
		TRACE(LOG_ERROR, "Connected...");
	}
	mysql_set_character_set(conn_, "utf8");

	return 0;
}

int ChatRegister::reConnectDB()
{
	//mysql_free_result(res);
	mysql_close(conn_);
	TRACE(LOG_ERROR, "reConnectDB mysql......");

	initDB();
}

void ChatRegister::getParameter(const string &keyWord, const string &query, string &reply)
{
	size_t valuePos = query.find(keyWord.c_str());
	if (string::npos == valuePos)
	{
		reply.clear();
		return;
	}
	size_t markPos = query.find_first_of("&", valuePos);

	valuePos += keyWord.length();
	reply = query.substr(valuePos, markPos - valuePos);
}

int ChatRegister::parseQuery(const string &query)
{
	string buf;

	//获取标签类型
	getParameter("userInfo=", query, buf);
	if (buf.empty())
	{
		TRACE(LOG_ERROR, "userInfo is empty");
		return -5;
	}
	wxInfo_.userInfo_ = buf.c_str();
	buf.clear();

	//获取code
	getParameter("code=", query, buf);
	if (buf.empty())
	{
		TRACE(LOG_ERROR, "code is empty");
		return -5;
	}
	wxInfo_.code_ = buf.c_str();
	buf.clear();

	//获取时间戳
	getParameter("stampTime=", query, buf);
	if (buf.empty())
	{
		TRACE(LOG_ERROR, "stampTime is empty");
		return -5;
	}
	wxInfo_.stampTime_ = buf;
	buf.clear();

	//获取签名
	getParameter("sign=", query, buf);
	if (buf.empty())
	{
		TRACE(LOG_ERROR, "sign is empty");
		return -5;
	}
	wxInfo_.sign_ = buf;
	buf.clear();

	getParameter("callback=", query, buf);
	callback_ = buf;
	buf.clear();

	TRACE(LOG_TRACE, "ChatRegister::parseQuery userInfo_: " << wxInfo_.userInfo_
						<< "   code: "		<< wxInfo_.code_
						<< "   stampTime: "	<< wxInfo_.stampTime_
						<< "   sign: "		<< wxInfo_.sign_
						<< "   callback: "	<< callback_);
	return 0;
}

int ChatRegister::parseUserInfo(UserInfo &userInfo)
{
	Json::Value value;
	Json::Reader reader;
	string decodeUrlInfo = CHttpclient::UrlDecode(wxInfo_.userInfo_);

	reader.parse(decodeUrlInfo.c_str(), value);

	if (value.isNull())
	{
		TRACE(LOG_ERROR, "userInfo is null");
		return -1;
	}
	userInfo.nickName_	= value.get("nickName", "").asString();
	userInfo.sex_		= value.get("gender", 0).asInt();
	userInfo.photo_		= value.get("avatarUrl", "").asString();
	userInfo.city_		= value.get("city", "").asString();

	return 0;
}

int ChatRegister::getSessionKey(UserInfo &userInfo)
{
	stringstream ssUrl;
	ssUrl << wxSessionUrl_ << "?appid=" << appid_
		  << "&secret=" << appSecret_
		  << "&js_code=" << wxInfo_.code_
		  << "&grant_type=authorization_code";

	Data data;
	CHttp http;
	http.HttpGet(ssUrl.str().c_str(), data);

	Json::Value jsonObj;
	stringstream iss((const char*)data.Buffer());
	try
	{
		iss >> jsonObj;
	}
	catch (std::exception &e)
	{
		TRACE(5, "getSessionKey error, exception: " << e.what() 
				<< "    data.Buffer: " << data.Buffer());
		return -1;
	}
	userInfo.openid_ = jsonObj.get("openid", "").asString();
	userInfo.unionid_ = jsonObj.get("unionid", "").asString();
	userInfo.sessionKey_ = jsonObj.get("session_key", "").asString();
	if (userInfo.openid_.size() == 0)
	{
		TRACE(LOG_ERROR, "openid is empty, data.Buffer: " << data.Buffer());
		return -1;
	}
	TRACE(LOG_ERROR, "data.Buffer: " << data.Buffer());
	return 0;
}

int ChatRegister::checkUser(const UserInfo &userInfo)
{
	stringstream sql;
	sql << "select 1 from t_user_profile where wx_id = '"
		<< userInfo.openid_
		<< "'";

	TRACE(LOG_ERROR, "ChatRegister::checkUser, sql: " << sql.str());

	MYSQL_RES *res;
	MYSQL_ROW row;
	int ret = 0;

	int t = 0;

	try
	{
		t = mysql_real_query(conn_, sql.str().c_str(), sql.str().size());
		if(t)
		{
			TRACE(LOG_ERROR, "ChatRegister::checkUser dbcmd(dbproc, sql.str().c_str()) error! ");
			reConnectDB();
			return -4;
		}
		else
		{
			res=mysql_use_result(conn_);
			if(res)
			{
				while((row=mysql_fetch_row(res))!=NULL)
				{
					ret = atoi(row[0]);
				}
			}
			mysql_free_result(res);
		}
	} 
	catch (...)
	{
		TRACE(LOG_ERROR, "database ChatRegister exception ");
		reConnectDB();
		return -3;
	}

	return ret;
}

int ChatRegister::insertMysql(const UserInfo &userInfo)
{
	stringstream sql;
	sql << "insert into t_user_profile (wx_id, nick_name, photo, sex, ctime, city, wx_unionid, session_key) value('"
		<< userInfo.openid_
		<< "', '"
		<< userInfo.nickName_
		<< "', '"
		<< userInfo.photo_
		<< "', "
		<< userInfo.sex_
		<< ", now(), '"
		<< userInfo.city_
		<< "', '"
		<< userInfo.unionid_
		<< "', '"
		<< userInfo.sessionKey_
		<< "')";

	TRACE(LOG_ERROR, "ChatRegister::insertMysql, sql: " << sql.str());

	int t = 0;

	try
	{
		t = mysql_real_query(conn_, sql.str().c_str(), sql.str().size());
		if(t)
		{
			TRACE(LOG_ERROR, "ChatRegister::insertMysql dbcmd(dbproc, sql.str().c_str()) error! ");
			reConnectDB();
			return -4;
		}
		else
		{
			//
		}
	} 
	catch (...)
	{
		TRACE(LOG_ERROR, "database ChatRegister exception ");
		reConnectDB();
		return -3;
	}

	return 0;
}

int ChatRegister::updateMysql(const UserInfo &userInfo)
{
	stringstream sql;
	sql << "update t_user_profile set photo = '"
		<< userInfo.photo_
		<< "', nick_name = '"
		<< userInfo.nickName_
		<< "', session_key = '"
		<< userInfo.sessionKey_
		<< "' where wx_id = '"
		<< userInfo.openid_
		<< "'";

	TRACE(LOG_ERROR, "ChatRegister::updateMysql, sql: " << sql.str());

	int t = 0;

	try
	{
		t = mysql_real_query(conn_, sql.str().c_str(), sql.str().size());
		if(t)
		{
			TRACE(LOG_ERROR, "ChatRegister::updateMysql dbcmd(dbproc, sql.str().c_str()) error! ");
			reConnectDB();
			return -4;
		}
		else
		{
			//
		}
	} 
	catch (...)
	{
		TRACE(LOG_ERROR, "database ChatRegister exception ");
		reConnectDB();
		return -3;
	}

	return 0;
}

int ChatRegister::getUserUid(Json::Value &jsonData, UserInfo &userInfo)
{
	stringstream sql;
	sql << "select user_idx from t_user_profile where wx_id = '"
		<< userInfo.openid_
		<< "'";

	TRACE(LOG_ERROR, "ChatRegister::getUserUid, sql: " << sql.str());

	MYSQL_RES *res;
	MYSQL_ROW row;

	int t = 0;

	try
	{
		t = mysql_real_query(conn_, sql.str().c_str(), sql.str().size());
		if(t)
		{
			TRACE(LOG_ERROR, "ChatRegister::getUserUid dbcmd(dbproc, sql.str().c_str()) error! ");
			reConnectDB();
			return -4;
		}
		else
		{
			res = mysql_use_result(conn_);
			if (res)
			{
				while ((row = mysql_fetch_row(res)) != NULL)
				{
					jsonData["user_idx"] = atoi(row[0]);
					userIdx_	= atoi(row[0]);
				}
			}
			mysql_free_result(res);
		}
	} 
	catch (...)
	{
		TRACE(LOG_ERROR, "database ChatRegister exception ");
		reConnectDB();
		return -3;
	}

	return 0;
}

int ChatRegister::insertStatus()
{
	stringstream sql;
	sql << "insert t_online_status (user_idx, room_id, online_status) value("
		<< userIdx_
		<< ", 0, "
		<< DOWN_LINE
		<< ")";

	TRACE(LOG_ERROR, "ChatRegister::insertStatus, sql: " << sql.str());

	int t = 0;

	try
	{
		t = mysql_real_query(conn_, sql.str().c_str(), sql.str().size());
		if(t)
		{
			TRACE(LOG_ERROR, "ChatRegister::insertStatus dbcmd(dbproc, sql.str().c_str()) error! ");
			reConnectDB();
			return -4;
		}
		else
		{
		}
	} 
	catch (...)
	{
		TRACE(LOG_ERROR, "database ChatRegister exception ");
		reConnectDB();
		return -3;
	}

	return 0;
}

void ChatRegister::clean()
{
	callback_	= "";
	userIdx_	= 0;
	wxInfo_.cleanWxInfo();
}
