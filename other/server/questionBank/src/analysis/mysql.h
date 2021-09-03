#pragma once
# include "analysis.h"

# include <string>
# include <sstream>
# include <map>
# include <mysql/mysql.h>

class Mysql
{
public:
	Mysql();
	~Mysql();
	
	//初始化连接数据库参数 
	void	init(const string , const string , const string , const string , const string );

	//初始化连接数据库
	int 	initDB();

	//执行数据库
	int insertMysql(VectorQuestion &question);

	//插入题干
	int insertStemMysql(const VectorQuestion &question);
	//插入答案
	int insertAnswerMysql(const VectorQuestion &question);
	//插入选项
	int insertOptionMysql(const VectorQuestion &question);
	//获取题干id
	int getStemId(VectorQuestion &question);

	//重连数据库
	int reConnectDB();

private:
	string		mip_;
	string		mport_;
	string		mdbName_;
	string		mdbUser_;
	string		mdbUserPwd_;

	MYSQL		*conn_;///连接数据库的句柄
};

