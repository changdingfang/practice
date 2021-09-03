# include "bbDetails.h"
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

BbDetails::BbDetails() : mip_(""), mport_("")
							   , mdbName_(""), mdbUser_("")
							   , mdbUserPwd_("") 
							   , conn_(NULL)
{

}

BbDetails::~BbDetails()
{
	if (NULL != conn_)
	{
		mysql_close(conn_);
		conn_ = NULL;
	}
}

int BbDetails::messageHandle(const string &request, string &response, string &callback)
{
	clean();
	int ret = parseQuery(request);
	if (0 != ret)
	{
		TRACE(LOG_ERROR, "parseQuery error, ret: " << ret << ", request: " << request);
		return -1;
	}
	callback = callback_;

	Json::Value jsonSummary;
	ret = getSummary(jsonSummary);
	if (0 != ret)
	{
		TRACE(LOG_ERROR, "BbDetails::messageHandle error, ret: " << ret << ", request: " << request);
		return -4;
	}

	ret = getSummaryAdditional(jsonSummary);
	if (0 != ret)
	{
		TRACE(LOG_ERROR, "BbDetails::messageHandle error, ret: " << ret << ", request: " << request);
		return -4;
	}

	if (bbInfo_.type_ >= 121 && bbInfo_.type_ <= 132)
	{//原型防御的type范围
		ret = getProtoTypeDefenses();
		if (0 != ret)
		{
			TRACE(LOG_ERROR, "BbDetails::messageHandle error, ret: " << ret << ", request: " << request);
			return -4;
		}
	}
	else
	{
		ret = getBbDetails();
		if (0 != ret)
		{
			TRACE(LOG_ERROR, "BbDetails::messageHandle error, ret: " << ret << ", request: " << request);
			return -4;
		}
	}

	//按照等级排序
	sort(dataVec_.begin(), dataVec_.end());

	ret = getBbDetailsAdditional();
	if (0 != ret)
	{
		TRACE(LOG_ERROR, "BbDetails::messageHandle error, ret: " << ret << ", request: " << request);
		return -4;
	}

	Json::Value jsonObject;
	ret = pack(jsonObject);
	if (0 != ret)
	{
		TRACE(LOG_ERROR, "BbDetails::messageHandle error, ret: " << ret << ", request: " << request);
		return -4;
	}
	jsonObject["summary"] = jsonSummary;

	Json::Value jsonValue;
	jsonValue["code"]      = 0;
	jsonValue["msg"]       = "success";
	jsonValue["info"]	   = jsonObject;
	Json::FastWriter fastWriter;
	response = fastWriter.write(jsonValue);

	return 0;
}

string BbDetails::trim(string& s)
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

void BbDetails::readDBConfig()
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

int BbDetails::initDB()
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

int BbDetails::reConnectDB()
{
	//mysql_free_result(res);
	mysql_close(conn_);
	TRACE(LOG_ERROR, "reConnectDB mysql......");

	initDB();
}

void BbDetails::getParameter(const string &keyWord, const string &query, string &reply)
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

int BbDetails::parseQuery(const string &query)
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

	TRACE(LOG_TRACE, "BbDetails::parseQuery type: " << bbInfo_.type_
						<< "   stampTime: "	<< bbInfo_.stampTime_
						<< "   sign: "		<< bbInfo_.sign_
						<< "   callback: "	<< callback_);
}

int BbDetails::getSummary(Json::Value &jsonSummary)
{
	stringstream sql;

	sql << "select content from t_summary where type = "
		<< bbInfo_.type_
		<< ";";

	TRACE(LOG_ERROR, "BbDetails::getSummary, sql: " << sql.str());

	MYSQL_RES *res;
	MYSQL_ROW row;

	int t = 0;

	try
	{
		t = mysql_real_query(conn_, sql.str().c_str(), sql.str().size());
		if(t)
		{
			TRACE(LOG_ERROR, "BbDetails::getSummary dbcmd(dbproc, sql.str().c_str()) error! ");
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
					jsonSummary["content"] = row[0];
				}
			}
			mysql_free_result(res);
		}
	} 
	catch (...)
	{
		TRACE(LOG_ERROR, "database getSummary exception ");
		reConnectDB();
		return -3;
	}

	return 0;
}

int BbDetails::getSummaryAdditional(Json::Value &jsonSummary)
{
	stringstream sql;

	sql << "select name, value from t_summary_additional where type = "
		<< bbInfo_.type_
		<< ";";

	TRACE(LOG_ERROR, "BbDetails::getSummaryAdditional, sql: " << sql.str());

	MYSQL_RES *res;
	MYSQL_ROW row;

	int t = 0;

	try
	{
		t = mysql_real_query(conn_, sql.str().c_str(), sql.str().size());
		if(t)
		{
			TRACE(LOG_ERROR, "BbDetails::getSummaryAdditional dbcmd(dbproc, sql.str().c_str()) error! ");
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
					jsonTmp["name"] = row[0];
					jsonTmp["value"] = row[1];
					jsonSummary["additional"].append(jsonTmp);
				}
			}
			mysql_free_result(res);
		}
	} 
	catch (...)
	{
		TRACE(LOG_ERROR, "database getSummaryAdditional exception ");
		reConnectDB();
		return -3;
	}

	return 0;
}

int BbDetails::getBbDetails()
{
	stringstream sql;

	sql << "select level, life, need_lev, wood, stone, steel, need_time, exp, gold "
		<< " from t_data where type = "
		<< bbInfo_.type_
		<< ";";

	TRACE(LOG_ERROR, "BbDetails::getBbDetails, sql: " << sql.str());

	MYSQL_RES *res;
	MYSQL_ROW row;

	int t = 0;

	try
	{
		t = mysql_real_query(conn_, sql.str().c_str(), sql.str().size());
		if(t)
		{
			TRACE(LOG_ERROR, "BbDetails::getBbDetails dbcmd(dbproc, sql.str().c_str()) error! ");
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
					Json::Value jsonBaseInfo;
					Json::Value jsonBuildInfo;
					Json::Value jsonBuildTime;

					Json::Value jsonBaseExp;
					Json::Value jsonBaseLev;
					Json::Value jsonBaseNeedLev;
					Json::Value jsonBaseLifeLev;

					jsonBaseExp["name"]		= "升级获得经验值";
					jsonBaseExp["value"]	= atoi(row[7]);

					jsonBaseLev["name"]		= "等级";
					jsonBaseLev["value"]	= atoi(row[0]);

					if (bbInfo_.type_ >= 81 && bbInfo_.type_ <= 110)
					{
						jsonBaseNeedLev["name"]	= "研究所所需等级";
						jsonBaseNeedLev["value"]= atoi(row[2]);
					}
					else
					{
						jsonBaseNeedLev["name"]	= "司令部所需等级";
						jsonBaseNeedLev["value"]= atoi(row[2]);
					}

					jsonBaseLifeLev["name"]	= "生命值";
					jsonBaseLifeLev["type"] = atoi(row[1]);

					jsonBaseInfo["exp"]		= jsonBaseExp;
					jsonBaseInfo["level"]	= jsonBaseLev;
					jsonBaseInfo["life"]	= jsonBaseLifeLev;
					jsonBaseInfo["need_lev"]	= jsonBaseNeedLev;


					if (atoi(row[3]) == -1)
					{
						jsonBuildInfo["gold"]		= atoi(row[8]);
					}
					else
					{
						jsonBuildInfo["wood"]		= atoi(row[3]);
						jsonBuildInfo["stone"]	= atoi(row[4]);
						jsonBuildInfo["steel"]	= atoi(row[5]);
					}

					jsonBuildTime["need_time"]= row[6];
					jsonBuildTime["name"]= "升级所需时间";


					jsonTmp["base_ifno"] = jsonBaseInfo;
					jsonTmp["build_info"] = jsonBuildInfo;
					jsonTmp["build_time"] = jsonBuildTime;

					dataVec_.push_back(make_pair(atoi(row[0]), jsonTmp));
				}
			}
			mysql_free_result(res);
		}
	} 
	catch (...)
	{
		TRACE(LOG_ERROR, "database getBbDetails exception ");
		reConnectDB();
		return -3;
	}

	return 0;
}

int BbDetails::getBbDetailsAdditional()
{
	stringstream sql;

	sql << "select name, value, level from t_additional where type = "
		<< bbInfo_.type_
		<< ";";

	TRACE(LOG_ERROR, "BbDetails::getBbDetailsAdditional, sql: " << sql.str());

	MYSQL_RES *res;
	MYSQL_ROW row;

	int t = 0;

	try
	{
		t = mysql_real_query(conn_, sql.str().c_str(), sql.str().size());
		if(t)
		{
			TRACE(LOG_ERROR, "BbDetails::getBbDetailsAdditional dbcmd(dbproc, sql.str().c_str()) error! ");
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
					jsonTmp["name"]	= row[0];
					jsonTmp["value"]= row[1];
					dataVec_[atoi(row[2]) - 1].second["additional"].append(jsonTmp);
				}
			}
			mysql_free_result(res);
		}
	} 
	catch (...)
	{
		TRACE(LOG_ERROR, "database getBbDetailsAdditional exception ");
		reConnectDB();
		return -3;
	}

	return 0;
}

int BbDetails::getProtoTypeDefenses()
{
	stringstream sql;

	sql << "select level, life, need_lev, fuses, gears, rods, capacitors, build_time "
		<< " from t_prototype_defenses where type = "
		<< bbInfo_.type_
		<< ";";

	TRACE(LOG_ERROR, "BbDetails::getProtoTypeDefenses, sql: " << sql.str());

	MYSQL_RES *res;
	MYSQL_ROW row;

	int t = 0;

	try
	{
		t = mysql_real_query(conn_, sql.str().c_str(), sql.str().size());
		if(t)
		{
			TRACE(LOG_ERROR, "BbDetails::getProtoTypeDefenses dbcmd(dbproc, sql.str().c_str()) error! ");
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
				//level, life, need_lev, fuses, gears, rods, capacitors, build_time "
					Json::Value jsonTmp;
					Json::Value jsonBaseInfo;
					Json::Value jsonBuildInfo;
					Json::Value jsonBuildTime;

					Json::Value jsonBaseLev;
					Json::Value jsonBaseNeedLev;
					Json::Value jsonBaseLifeLev;

					jsonBaseLev["name"]		= "等级";
					jsonBaseLev["value"]	= atoi(row[0]);

					jsonBaseNeedLev["name"]	= "实验室所需等级";
					jsonBaseNeedLev["value"]= atoi(row[2]);

					jsonBaseLifeLev["name"]	= "生命值";
					jsonBaseLifeLev["type"] = atoi(row[1]);

					jsonBaseInfo["level"]	= jsonBaseLev;
					jsonBaseInfo["life"]	= jsonBaseLifeLev;
					jsonBaseInfo["need_lev"]	= jsonBaseNeedLev;

					jsonBuildInfo["fuses"]	= atoi(row[3]);
					jsonBuildInfo["gears"]	= atoi(row[4]);
					jsonBuildInfo["rods"]		= atoi(row[5]);
					jsonBuildInfo["capacitors"]= row[6];

					jsonBuildTime["need_time"]= row[7];
					jsonBuildTime["name"]= "建造所需时间";


					jsonTmp["base_ifno"] = jsonBaseInfo;
					jsonTmp["build_info"] = jsonBuildInfo;
					jsonTmp["build_time"] = jsonBuildTime;

					dataVec_.push_back(make_pair(atoi(row[0]), jsonTmp));
				}
			}
			mysql_free_result(res);
		}
	} 
	catch (...)
	{
		TRACE(LOG_ERROR, "database getProtoTypeDefenses exception ");
		reConnectDB();
		return -3;
	}

	return 0;
}

int BbDetails::pack(Json::Value &jsonObject)
{
	for (int i = 0; i < dataVec_.size(); ++i)
	{
		jsonObject["data"].append(dataVec_[i].second);
	}
}

void BbDetails::clean()
{
	dataVec_.clear();
	callback_ = "";
	bbInfo_.cleanBbInfo();
}
