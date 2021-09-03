# include "chatGetHistoryChat.h"
# include "common/log_tools.h"

# include <time.h>

# include <iostream>
# include <fstream>

RequestInfo::RequestInfo()
{
}

RequestInfo::~RequestInfo()
{

}

HistoryChat::HistoryChat() : mip_(""), mport_("")
							   , mdbName_(""), mdbUser_("")
							   , mdbUserPwd_("") 
							   , conn_(NULL)
{
}

HistoryChat::~HistoryChat()
{
	if (NULL != conn_)
	{
		mysql_close(conn_);
		conn_ = NULL;
	}
}

int HistoryChat::messageHandle(const string &request, string &response, string &callback)
{
	clean();
	int ret = parseQuery(request);
	if (0 != ret)
	{
		TRACE(LOG_ERROR, "parseQuery error, ret: " << ret << ", request: " << request);
		return -1;
	}
	callback = callback_;

	Json::Value jsonData;
	ret = getHistoryChat(jsonData);
	if (0 != ret)
	{
		TRACE(LOG_ERROR, "HistoryChat::messageHandle error, ret: " << ret << ", request: " << request);
		return -4;
	}

	Json::Value jsonValue;
	jsonValue["code"]      = 0;
	jsonValue["msg"]       = "success";
	jsonValue["info"]	   = jsonData;
	Json::FastWriter fastWriter;
	response = fastWriter.write(jsonValue);

	return 0;
}

string HistoryChat::trim(string& s)
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

void HistoryChat::readDBConfig()
{
	ifstream is_file("conf.conf");
	if (!is_file.is_open())
	{
		TRACE(LOG_ERROR,  "Cannot find config: conf.conf" );
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
			}
		}
	}
}

int HistoryChat::initDB()
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

int HistoryChat::reConnectDB()
{
	//mysql_free_result(res);
	mysql_close(conn_);
	TRACE(LOG_ERROR, "reConnectDB mysql......");

	initDB();
}

void HistoryChat::getParameter(const string &keyWord, const string &query, string &reply)
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

int HistoryChat::parseQuery(const string &query)
{
	string buf;

	//获取标签类型
	getParameter("userIdx=", query, buf);
	if (buf.empty())
	{
		TRACE(LOG_ERROR, "userIdx is empty");
		return -5;
	}
	requestInfo_.userIdx_ = atoi(buf.c_str());
	buf.clear();

	//获取时间戳
	getParameter("stampTime=", query, buf);
	if (buf.empty())
	{
		TRACE(LOG_ERROR, "stampTime is empty");
		return -5;
	}
	requestInfo_.stampTime_ = buf;
	buf.clear();

	//获取签名
	getParameter("sign=", query, buf);
	if (buf.empty())
	{
		TRACE(LOG_ERROR, "sign is empty");
		return -5;
	}
	requestInfo_.sign_ = buf;
	buf.clear();

	//获取pageNum
	getParameter("pageNum=", query, buf);
	if (buf.empty())
	{
		TRACE(LOG_ERROR, "pageNum is empty");
		return -5;
	}
	requestInfo_.pageNum_ = atoi(buf.c_str());
	//暂时默认一页是ONLINE_USER_PAGE_NUM条数据
	//if (0 == requestInfo_.pageNum_)
	{
		requestInfo_.pageNum_ = HISTORY_CHAT_PAGE_NUM;
	}
	buf.clear();

	//获取page
	getParameter("page=", query, buf);
	if (buf.empty())
	{
		TRACE(LOG_ERROR, "page is empty");
		return -5;
	}
	requestInfo_.page_ = atoi(buf.c_str());
	buf.clear();

	//获取房间id
	getParameter("roomId=", query, buf);
	if (buf.empty())
	{
		TRACE(LOG_ERROR, "roomId is empty");
		return -5;
	}
	requestInfo_.roomId_ = atoi(buf.c_str());
	buf.clear();

	getParameter("callback=", query, buf);
	callback_ = buf;
	buf.clear();

	TRACE(LOG_TRACE, "HistoryChat::parseQuery userIdx: " << requestInfo_.userIdx_
						<< "   stampTime: "	<< requestInfo_.stampTime_
						<< "   sign: "		<< requestInfo_.sign_
						<< "   pageNUm: "	<< requestInfo_.pageNum_
						<< "   page: "		<< requestInfo_.page_
						<< "   roomId: "	<< requestInfo_.roomId_
						<< "   callback: "	<< callback_);
}

int	HistoryChat::getHistoryChat(Json::Value &jsonData)
{
	stringstream sql;

	sql << "select H.user_idx, P.nick_name, P.photo, H.content, H.type, "
		<< " H.ctime from t_history_chat as H "
		<< " inner join t_user_profile as P on P.user_idx = H.user_idx where H.room_id = "
		<< requestInfo_.roomId_
		<< " order by ctime desc limit ";
	int begPage = requestInfo_.page_ * requestInfo_.pageNum_;
	//int endPage = (requestInfo_.page_ + 1) * requestInfo_.pageNum_ + USER_IDX_BASE - 1;
	sql << begPage << ", "
		<< requestInfo_.pageNum_;

	TRACE(LOG_ERROR, "getHistoryChat::getHistoryChat, sql: " << sql.str());

	MYSQL_RES *res;
	MYSQL_ROW row;

	int t = 0;

	try
	{
		t = mysql_real_query(conn_, sql.str().c_str(), sql.str().size());
		if(t)
		{
			TRACE(LOG_ERROR, "HistoryChat::getHistoryChat dbcmd(dbproc, sql.str().c_str()) error! sql: " << sql.str());
			reConnectDB();
			return -4;
		}
		else
		{
			res=mysql_use_result(conn_);
			if(res)
			{
				while((row = mysql_fetch_row(res)) != NULL)
				{
					Json::Value jsonTmp;
					jsonTmp["user_idx"]		= atoi(row[0]);
					jsonTmp["nick_name"]	= row[1];
					jsonTmp["photo"]		= row[2];
					jsonTmp["content"]		= row[3];
					jsonTmp["type"]			= atoi(row[4]);
					jsonTmp["ctime"]		= row[5];
					jsonData.append(jsonTmp);
				}
			}
			mysql_free_result(res);
		}
	} 
	catch (...)
	{
		TRACE(LOG_ERROR, "database getHistoryChat exception sql: " << sql.str());
		reConnectDB();
		return -3;
	}

	return 0;
}

void HistoryChat::clean()
{
	callback_ = "";
	requestInfo_.cleanRequestInfo();
}
