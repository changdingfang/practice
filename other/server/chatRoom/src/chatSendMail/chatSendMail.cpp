#include "chatSendMail.h"
#include "common/log_tools.h"
#include "/usr/include/python2.7/Python.h"

#include <time.h>

#include <iostream>
#include <fstream>

SendMail::SendMail() : mip_(""), mport_("")
							   , mdbName_(""), mdbUser_("")
							   , mdbUserPwd_("") 
							   , conn_(NULL)
{

}

SendMail::~SendMail()
{
	if (NULL != conn_)
	{
		mysql_close(conn_);
		conn_ = NULL;
	}
}


int SendMail::messageHandle(const string &request, string &response)
{
	clean();
	string msg = "success";
	int code = 0;

	int ret = parseQuery(request);
	if (0 != ret)
	{
		TRACE(LOG_ERROR, "parseQuery error, ret: " << ret << ", request: " << request);
		return -1;
	}

	ret = checkMail();
	if (1 == ret)
	{
		msg = "This mailbox has been registered.";
		code = 100;
	}
	else if (0 == ret)
	{
		ret = insertMysql();
		if (0 != ret)
		{
			TRACE(LOG_ERROR, "SendMail::messageHandle error, ret: " << ret << ", request: " << request);
			return -1;
		}
		ret = sendMail();
		if (ret != 0)
		{
			TRACE(LOG_ERROR, "SendMail::messageHandle error, ret: " << ret << ", request: " << request);
			//如果发送邮件失败, 则删除数据库记录.暂时不实现
			//deleteMysql();
		}
	}
	else
	{
		TRACE(LOG_ERROR, "SendMail::messageHandle error, ret: " << ret << ", request: " << request);
		return -1;
	}
	
	Json::Value jsonData;

	Json::Value jsonValue;
	jsonValue["code"]	= code;
	jsonValue["msg"]	= msg;
	jsonValue["randNum"]= info_.randNum_;
	Json::FastWriter fastWriter;
	response = fastWriter.write(jsonValue);

	return 0;
}

string SendMail::trim(string& s)
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

void SendMail::readDBConfig()
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
				else if (key == "python_path")
				{
					this->pythonPath_ = trim(value);
				}
				else if (key == "python_send_mail")
				{
					this->pythonFile_ = trim(value);
				}
			}
		}
	}
}

int SendMail::initDB()
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

int SendMail::reConnectDB()
{
	//mysql_free_result(res);
	mysql_close(conn_);
	TRACE(LOG_ERROR, "reConnectDB mysql......");

	initDB();
}

void SendMail::getParameter(const string &keyWord, const string &query, string &reply)
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

int SendMail::parseQuery(const string &query)
{
	string buf;

	//获取mail
	getParameter("mail=", query, buf);
	if (buf.empty())
	{
		TRACE(LOG_ERROR, "mail is empty");
		return -5;
	}
	info_.mail_ = buf.c_str();
	buf.clear();

	//获取时间戳
	getParameter("stampTime=", query, buf);
	if (buf.empty())
	{
		TRACE(LOG_ERROR, "stampTime is empty");
		return -5;
	}
	info_.stampTime_ = buf;
	buf.clear();

	//获取签名
	getParameter("sign=", query, buf);
	if (buf.empty())
	{
		TRACE(LOG_ERROR, "sign is empty");
		return -5;
	}
	info_.sign_ = buf;
	buf.clear();

	TRACE(LOG_TRACE, "SendMail::parseQuery mail: " << info_.mail_
						<< "   stampTime: "	<< info_.stampTime_
						<< "   sign: "		<< info_.sign_);
	return 0;
}

int SendMail::checkMail()
{
	stringstream sql;
	sql << "select user_idx from t_user_profile where register_type = 2 and wx_id = '"
		<< info_.mail_
		<< "'";

	MYSQL_RES *res;
	MYSQL_ROW row;


	int t = 0;

	try
	{
		t = mysql_real_query(conn_, sql.str().c_str(), sql.str().size());
		if(t)
		{
			TRACE(LOG_ERROR, "SendMail::checkMail dbcmd(dbproc, sql.str().c_str()) error! sql: " << sql.str());
			reConnectDB();
			return -4;
		}
		else
		{
			res=mysql_use_result(conn_);
			if(res)
			{
				while((row=mysql_fetch_row(res)) != NULL)
				{
					if (atoi(row[0]) > 0)
					{
						return 1;
					}
				}
			}
		}
	} 
	catch (...)
	{
		TRACE(LOG_ERROR, "database checkMail exception sql: " << sql.str());
		reConnectDB();
		return -3;
	}

	return 0;
}

int SendMail::insertMysql()
{
	srand(time(NULL));
	int randNum = rand() % 100000;
	stringstream randNumSs;
	for (int i = 0; i < 6; ++i)
	{
		randNum = rand() % 10;
		randNumSs << randNum;
	}
	info_.randNum_ = randNumSs.str();

	stringstream sql;
	sql << "insert into t_rand_num(register_mail, rand_num, ctime) value('"
		<< info_.mail_
		<< "', '"
		<< info_.randNum_
		<< "', now())";

	TRACE(LOG_ERROR, "SendMail::insertMysql, sql: " << sql.str());

	int t = 0;

	try
	{
		t = mysql_real_query(conn_, sql.str().c_str(), sql.str().size());
		if(t)
		{
			TRACE(LOG_ERROR, "SendMail::insertMysql dbcmd(dbproc, sql.str().c_str()) error! ");
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
		TRACE(LOG_ERROR, "database SendMail exception ");
		reConnectDB();
		return -3;
	}

	return 0;
}

int SendMail::sendMail()
{
	//初始化python
	Py_Initialize();

	if (!Py_IsInitialized())
	{
		TRACE(LOG_ERROR, "py_initialize() error");
		return -1;
	}

	PyRun_SimpleString("import sys");
	PyRun_SimpleString(pythonPath_.c_str());

	PyObject* get_envs_module = PyImport_ImportModule(pythonFile_.c_str());
	if (get_envs_module == NULL)
	{
		TRACE(LOG_ERROR, "PyImport_ImportModule error, get_envs_module is null");
		return -1;
	}
	PyObject* get_envs_func = PyObject_GetAttrString(get_envs_module, "send_mail");
	if (get_envs_func == NULL)
	{
		TRACE(LOG_ERROR, "PyObject_GetAttrString error, get_envs_func is null");
		return -1;
	}
	//创建两个参数
	PyObject *args = PyTuple_New(2);
	PyTuple_SetItem(args, 0, Py_BuildValue("s", info_.mail_.c_str()));
	PyTuple_SetItem(args, 1, Py_BuildValue("s", info_.randNum_.c_str()));

	PyObject *func_ret_val = PyObject_CallObject(get_envs_func, args);

	return 0;
}

void SendMail::clean()
{
	info_.cleanInfo();
}
