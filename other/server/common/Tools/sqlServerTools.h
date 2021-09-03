/*
 *作者：刘洋
 *日期：2016-11-15
 *描述：封装对SQLServer的操作
 * 版本：1.0.0.1
 * 说明：高版本可以取代低版本，反之不可以。1.0.0.0 > 没有版本号
 * */
#ifndef SQLSERVERTOOLS_H__
#define SQLSERVERTOOLS_H__

#include <sybfront.h>
#include <sybdb.h>
#include <string>
#include <stdlib.h>
#include <stdint.h>

#include "errorTools.h"
#include "../json/json.h"

typedef bool (*HandDBbind) (DBPROCESS *, Json::Value &);
typedef void* (*retHandle)(DBPROCESS *prec);

class SqlServerTools{
public:
	//构造函数
	SqlServerTools(const std::string &dbip, const uint16_t dbport, const std::string &userName,
						const std::string &userPassword, const std::string &dbName);
	
	//重载无参构造函数
	//SqlServerTools(void);
	
	~SqlServerTools(void);
	
	//连接数据库
	bool dbInit(const char *charset = "GB2312");
	
	//执行查询语句
	//参数：db_hand: 回调参数
	//	sql: sql语句
	//	json: 查询结果
	int dbSelect(const std::string &sql, HandDBbind db_hand, Json::Value &json);

	//查询数据库
	//描述：函数执行select 的SQL语句，回调函数用处理查询语句返回的结果
	//sql ：sql语句
	//p_hand: 回调函数，void* (*retHandle)(DBPROCESS *prec);
	int dbSelect(const std::string &sql, retHandle p_hand);
	
	//执行sql语句
	int dbQuery(const std::string &sql);

	//获取数据库连接状态
	bool getDBstat(void);
	
	//关闭数据库
	bool dbClose(void);
private:
	std::string     m_dbip; //slq数据库服务器IP
	uint16_t        m_dbport; //sql数据库端口号
	std::string     m_userName; //数据库用户名
	std::string     m_userPassword; //数据库密码
	std::string     m_dbName; //数据库名
	bool            m_dbProof;
	
	DBPROCESS       *m_DBProcess;
	LOGINREC        *m_loginrec;
	RETCODE         m_retCode;
	ErrorTools      m_errtool; //异常类
};

#endif //SQLSERVERTOOLS_H__
