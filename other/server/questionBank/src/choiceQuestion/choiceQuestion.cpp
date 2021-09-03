# include "choiceQuestion.h"
# include "common/log_tools.h"

# include <time.h>

# include <iostream>
# include <fstream>

UserInfo::UserInfo()
{
	page_		= 0;
	pageNum_	= 0;
}

UserInfo::~UserInfo()
{

}

ChoiceQuestion::ChoiceQuestion() : mip_(""), mport_("")
							   , mdbName_(""), mdbUser_("")
							   , mdbUserPwd_("") 
							   , conn_(NULL)
{

}

ChoiceQuestion::~ChoiceQuestion()
{
	if (NULL != conn_)
	{
		mysql_close(conn_);
		conn_ = NULL;
	}
}

int ChoiceQuestion::messageHandle(const string &request, string &response, string &callback)
{
	clean();
	int ret = parseQuery(request);
	if (0 != ret)
	{
		TRACE(LOG_ERROR, "parseQuery error, ret: " << ret << ", request: " << request);
		return -1;
	}
	callback = callback_;

	ret = getStem();
	if (0 != ret)
	{
		TRACE(LOG_ERROR, "ChoiceQuestion::messageHandle error, ret: " << ret << ", request: " << request);
		return -4;
	}

	ret = getAnswer();
	if (0 != ret)
	{
		TRACE(LOG_ERROR, "ChoiceQuestion::messageHandle error, ret: " << ret << ", request: " << request);
		return -4;
	}

	ret = getOption();
	if (0 != ret)
	{
		TRACE(LOG_ERROR, "ChoiceQuestion::messageHandle error, ret: " << ret << ", request: " << request);
		return -4;
	}

	Json::Value jsonObject;
	ret = pageJson(jsonObject);
	if (0 != ret)
	{
		TRACE(LOG_ERROR, "ChoiceQuestion::messageHandle error, ret: " << ret << ", request: " << request);
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

string ChoiceQuestion::trim(string& s)
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

void ChoiceQuestion::readDBConfig()
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
				else if (key == "db_name")
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

int ChoiceQuestion::initDB()
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

int ChoiceQuestion::reConnectDB()
{
	//mysql_free_result(res);
	mysql_close(conn_);
	TRACE(LOG_ERROR, "reConnectDB mysql......");

	initDB();
}

void ChoiceQuestion::getParameter(const string &keyWord, const string &query, string &reply)
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

int ChoiceQuestion::parseQuery(const string &query)
{
	string buf;

	//获取页码
	getParameter("page=", query, buf);
	if (buf.empty())
	{
		TRACE(LOG_ERROR, "page is empty");
		return -5;
	}
	userInfo_.page_ = atoi(buf.c_str());
	buf.clear();

	//获取一页数据量
	getParameter("pageNum=", query, buf);
	if (buf.empty())
	{
		TRACE(LOG_ERROR, "pageNum is empty");
		return -5;
	}
	userInfo_.pageNum_ = atoi(buf.c_str());
	buf.clear();

	getParameter("callback=", query, buf);
	callback_ = buf;
	buf.clear();

	TRACE(LOG_TRACE, "ChoiceQuestion::parseQuery page: " << userInfo_.page_
						<< "   pageNum: "	<< userInfo_.pageNum_
						<< "   callback: "	<< callback_);
}

int ChoiceQuestion::getStem()
{
	stringstream sql;
	if (0 == userInfo_.pageNum_)
	{
		TRACE(LOG_ERROR, "pageNum is 0");
		return -1;
	}
	int pageBeg = (userInfo_.page_ - 1 ) * userInfo_.pageNum_;
	int pageEnd = userInfo_.page_ * userInfo_.pageNum_;

	sql << "select stem_id, stem from t_test where stem_id between "
		<< pageBeg
		<< " and "
		<< pageEnd
		<< ";";

	TRACE(LOG_ERROR, "ChoiceQuestion::ChoiceQuestion, sql: " << sql.str());

	MYSQL_RES *res;
	MYSQL_ROW row;

	int t = 0;

	stringstream stemIdSs;

	try
	{
		t = mysql_real_query(conn_, sql.str().c_str(), sql.str().size());
		if(t)
		{
			TRACE(LOG_ERROR, "ChoiceQuestion::ChoiceQuestion dbcmd(dbproc, sql.str().c_str()) error! ");
			reConnectDB();
		}
		else
		{
			res=mysql_use_result(conn_);
			if(res)
			{
				while((row=mysql_fetch_row(res))!=NULL)
				{
					if (stemMap_.find(atoi(row[0])) == stemMap_.end())
					{
						Json::Value jsonTmp;
						jsonTmp["stem_id"]	= atoi(row[0]);
						jsonTmp["stem"]		= row[1];
						stemMap_[atoi(row[0])] = jsonTmp;

						stemIdSs << atoi(row[0]) << ",";
					}
				}
			}
			mysql_free_result(res);
		}
	} 
	catch (...)
	{
		TRACE(LOG_ERROR, "database ChoiceQuestion exception ");
		reConnectDB();
		return -3;
	}

	if (!stemIdSs.str().empty())
	{
		listStemId_ = stemIdSs.str().substr(0, stemIdSs.str().size() - 1);
	}

	return 0;
}

int ChoiceQuestion::getAnswer()
{
	stringstream sql;

	if (listStemId_.empty())
	{
		TRACE(LOG_ERROR, "ChoiceQuestion::getAnswer listStemId is empty !");
		return -1;
	}
	sql << "select answer, stem_id from t_test_answer where stem_id in("
		<< listStemId_
		<< ");";

	TRACE(LOG_ERROR, "ChoiceQuestion::ChoiceQuestion, sql: " << sql.str());

	MYSQL_RES *res;
	MYSQL_ROW row;

	int t = 0;

	try
	{
		t = mysql_real_query(conn_, sql.str().c_str(), sql.str().size());
		if(t)
		{
			TRACE(LOG_ERROR, "ChoiceQuestion::ChoiceQuestion dbcmd(dbproc, sql.str().c_str()) error! ");
			reConnectDB();
		}
		else
		{
			res=mysql_use_result(conn_);
			if(res)
			{
				while((row=mysql_fetch_row(res))!=NULL)
				{
					if (stemMap_.find(atoi(row[1])) != stemMap_.end())
					{
						Json::Value jsonTmp;
						jsonTmp["answer"]	= row[0];
						stemMap_[atoi(row[1])]["answer"].append(jsonTmp);
					}
				}
			}
			mysql_free_result(res);
		}
	} 
	catch (...)
	{
		TRACE(LOG_ERROR, "database ChoiceQuestion exception ");
		reConnectDB();
		return -3;
	}

	return 0;
}
	
int ChoiceQuestion::getOption()
{
	stringstream sql;

	if (listStemId_.empty())
	{
		TRACE(LOG_ERROR, "ChoiceQuestion::getOption listStemId is empty !");
		return -1;
	}
	sql << "select `option`, stem_id from t_test_option where stem_id in("
		<< listStemId_
		<< ");";

	TRACE(LOG_ERROR, "ChoiceQuestion::ChoiceQuestion, sql: " << sql.str());

	MYSQL_RES *res;
	MYSQL_ROW row;

	int t = 0;

	try
	{
		t = mysql_real_query(conn_, sql.str().c_str(), sql.str().size());
		if(t)
		{
			TRACE(LOG_ERROR, "ChoiceQuestion::ChoiceQuestion dbcmd(dbproc, sql.str().c_str()) error! ");
			reConnectDB();
		}
		else
		{
			res=mysql_use_result(conn_);
			if(res)
			{
				while((row=mysql_fetch_row(res))!=NULL)
				{
					if (stemMap_.find(atoi(row[1])) != stemMap_.end())
					{
						Json::Value jsonTmp;
						jsonTmp["option"]	= row[0];
						//stemMap_[atoi(row[1])]["option"].append(jsonTmp);
						stemMap_[atoi(row[1])]["option"].append(row[0]);
					}
				}
			}
			mysql_free_result(res);
		}
	} 
	catch (...)
	{
		TRACE(LOG_ERROR, "database ChoiceQuestion exception ");
		reConnectDB();
		return -3;
	}

	return 0;
}

int ChoiceQuestion::pageJson(Json::Value& jsonObject)
{
	try
	{
		map<int, Json::Value>::iterator it = stemMap_.begin();
		while (it != stemMap_.end())
		{
			jsonObject.append(it->second);
			++it;
		}
	}
	catch (...)
	{
		TRACE(LOG_ERROR, "pageJson error");
		return -2;
	}

	return 0;
}

void ChoiceQuestion::clean()
{
	userInfo_.cleanUserInfo();
	stemMap_.clear();
	listStemId_.clear();
}
