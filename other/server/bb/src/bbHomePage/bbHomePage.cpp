# include "bbHomePage.h"
# include "common/log_tools.h"

# include <time.h>

# include <iostream>
# include <fstream>

BbInfo::BbInfo()
{
}

BbInfo::~BbInfo()
{

}

BbHomePage::BbHomePage() : mip_(""), mport_("")
							   , mdbName_(""), mdbUser_("")
							   , mdbUserPwd_("") 
							   , conn_(NULL)
{

}

BbHomePage::~BbHomePage()
{
	if (NULL != conn_)
	{
		mysql_close(conn_);
		conn_ = NULL;
	}
}

int BbHomePage::messageHandle(const string &request, string &response, string &callback)
{
	clean();
	int ret = parseQuery(request);
	if (0 != ret)
	{
		TRACE(LOG_ERROR, "parseQuery error, ret: " << ret << ", request: " << request);
		return -1;
	}
	callback = callback_;

	Json::Value jsonObject;
	ret = getBbInfo(jsonObject);
	if (0 != ret)
	{
		TRACE(LOG_ERROR, "BbHomePage::messageHandle error, ret: " << ret << ", request: " << request);
		return -4;
	}

	Json::Value jsonValue;
	jsonValue["code"]      = 0;
	jsonValue["msg"]       = "success";
	jsonValue["info"]	   = jsonObject;
	Json::FastWriter fastWriter;
	response = fastWriter.write(jsonValue);

	return 0;
}

string BbHomePage::trim(string& s)
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

void BbHomePage::readDBConfig()
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
				else if (key == "db_bbname")
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
			}
		}
	}
}

int BbHomePage::initDB()
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

int BbHomePage::reConnectDB()
{
	//mysql_free_result(res);
	mysql_close(conn_);
	TRACE(LOG_ERROR, "reConnectDB mysql......");

	initDB();
}

void BbHomePage::getParameter(const string &keyWord, const string &query, string &reply)
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

int BbHomePage::parseQuery(const string &query)
{
	string buf;

	//获取标签类型
	getParameter("type=", query, buf);
	if (buf.empty())
	{
		TRACE(LOG_ERROR, "type is empty");
		return -5;
	}
	bbInfo_.type_ = atoi(buf.c_str());
	buf.clear();

	//获取时间戳
	getParameter("stampTime=", query, buf);
	if (buf.empty())
	{
		TRACE(LOG_ERROR, "stampTime is empty");
		return -5;
	}
	bbInfo_.stampTime_ = buf;
	buf.clear();

	//获取签名
	getParameter("sign=", query, buf);
	if (buf.empty())
	{
		TRACE(LOG_ERROR, "sign is empty");
		return -5;
	}
	bbInfo_.sign_ = buf;
	buf.clear();

	getParameter("callback=", query, buf);
	callback_ = buf;
	buf.clear();

	TRACE(LOG_TRACE, "BbHomePage::parseQuery type: " << bbInfo_.type_
						<< "   stampTime: "	<< bbInfo_.stampTime_
						<< "   sign: "		<< bbInfo_.sign_
						<< "   callback: "	<< callback_);
}

int BbHomePage::getBbInfo(Json::Value &jsonObject)
{
	stringstream sql;

	sql << "select type, up_type, `name` from catalog where up_type = "
		<< bbInfo_.type_
		<< ";";

	TRACE(LOG_ERROR, "BbHomePage::BbHomePage, sql: " << sql.str());

	MYSQL_RES *res;
	MYSQL_ROW row;

	int t = 0;

	try
	{
		t = mysql_real_query(conn_, sql.str().c_str(), sql.str().size());
		if(t)
		{
			TRACE(LOG_ERROR, "BbHomePage::BbHomePage dbcmd(dbproc, sql.str().c_str()) error! ");
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
					Json::Value jsonTmp;
					jsonTmp["type"]		= atoi(row[0]);
					jsonTmp["up_type"]	= row[1];
					jsonTmp["name"]		= row[2];
					jsonObject.append(jsonTmp);
				}
			}
			mysql_free_result(res);
		}
	} 
	catch (...)
	{
		TRACE(LOG_ERROR, "database BbHomePage exception ");
		reConnectDB();
		return -3;
	}

	return 0;
}

void BbHomePage::clean()
{
	callback_ = "";
	bbInfo_.cleanBbInfo();
}
