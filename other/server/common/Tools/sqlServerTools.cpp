#include "sqlServerTools.h"
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

using namespace std;

//构造函数
SqlServerTools::SqlServerTools(const string &dbip, const uint16_t dbport, const string &userName, const string &userPassword, 
                               const string &dbName): m_dbip(dbip),m_dbport(dbport),m_userName(userName),
				m_userPassword(userPassword), m_dbName(dbName), m_DBProcess(NULL),m_loginrec(NULL),m_dbProof(false)
{}

SqlServerTools::~SqlServerTools(void)
{
	dbClose();
}

//连接数据库
bool SqlServerTools::dbInit(const char *charset/*="GB18030"*/)
{
	if(m_dbProof)
	{
		dbClose();
	}
	//初始化数据库
	dbinit();
	//连接sqlServer
	m_loginrec = dblogin();
	if(m_loginrec == NULL)
	{
		m_errtool = "dblogin error";
		m_dbProof = false;
		throw(m_errtool);
	}
	DBSETLUSER(m_loginrec, m_userName.c_str()); //设置用户名
	DBSETLPWD(m_loginrec, m_userPassword.c_str()); //设置密码
	DBSETLCHARSET(m_loginrec, charset); //设置编码
	DBSETLVERSION(m_loginrec, DBVERSION_71); //设置版本
	
	stringstream sqlDBServer;
	sqlDBServer << m_dbip << ":" << m_dbport;
	if((m_DBProcess = dbopen(m_loginrec, sqlDBServer.str().c_str() )) == NULL) //连接数据库
	{
		//连接失败
		string msg("SqlServerTools::dbInit() The error is not connect to SqlServer: ");
		m_errtool = msg.c_str();
		m_dbProof = false;
		throw(m_errtool);
	}
	if(dbuse(m_DBProcess, m_dbName.c_str()) == FAIL)
	{
		//改变数据库失败
		m_errtool = "SqlServerTools::dbInit() dbuse error";
		m_dbProof = false;
		throw(m_errtool);
	}
	m_dbProof = true;
	return true; //数据库连接成功
}

//执行sql语句
int SqlServerTools::dbQuery(const string &sql)
{
	if(dbcmd(m_DBProcess, sql.c_str()) == FAIL) //添加sql语句
	{
		//添加sql语句错误
		m_errtool = "dbcmd failed";
		throw(m_errtool);
	}

	if((m_retCode = dbsqlexec(m_DBProcess)) == FAIL) //执行sql语句
	{
		stringstream tmp;
		tmp << (int)m_retCode << "dbsqlexec failed" << sql;
		//sql语句执行错误
		m_errtool = tmp.str().c_str();
		throw(m_errtool);
	}
	return m_retCode;
}

//执行查询语句
//参数：db_hand: 回调参数
//	sql: sql语句
//	json: 查询结果
int SqlServerTools::dbSelect(const std::string &sql, HandDBbind db_hand, Json::Value &json)
{
	dbQuery(sql); //执行SQL语句
	if( (m_retCode = dbresults(m_DBProcess)) != NO_MORE_RESULTS )
	{
		if(m_retCode == SUCCEED)
		{
			if(db_hand(m_DBProcess, json)) //处理执行结果
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			json.clear();
		}
	}
	else
	{
		json.clear();
	}

	return -1;
}
//查询数据库
int SqlServerTools::dbSelect(const std::string &sql, retHandle p_hand)
{
	//执行Sql语句
	dbQuery(sql);

	//执行回调函数并返回结果
	p_hand(m_DBProcess);
	return 1;
}

//关闭数据库

//获取数据库连接状态
bool SqlServerTools::getDBstat(void)
{
	return m_dbProof;
}

//关闭数据库
bool SqlServerTools::dbClose(void)
{
    if(m_DBProcess != NULL)
    {
        dbclose(m_DBProcess);
        dbloginfree(m_loginrec);
        dbexit();
        m_DBProcess = NULL;
	m_dbProof = false;
        return true;
    }
    return false;
}



