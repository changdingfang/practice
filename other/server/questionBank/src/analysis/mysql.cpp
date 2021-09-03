# include "mysql.h"

# include <stdlib.h>
# include <stdio.h>

Mysql::Mysql() : mip_(""), mport_("")
							   , mdbName_(""), mdbUser_("")
							   , mdbUserPwd_("") 
							   , conn_(NULL)
{

}

Mysql::~Mysql()
{
	if (NULL != conn_)
	{
		mysql_close(conn_);
		conn_ = NULL;
	}
}

void Mysql::init(const string mip, const string mport, const string mdbName, const string mdbUser, const string mdbUserPwd)
{
	mip_	 	= mip;
	mport_		= mport;
	mdbName_	= mdbName;
	mdbUser_	= mdbUser;
	mdbUserPwd_	= mdbUserPwd;

	initDB();
}

int Mysql::initDB()
{
	//初始化
	conn_ = mysql_init(NULL);

	//连接数据库 
	if(!mysql_real_connect(conn_, mip_.c_str(), mdbUser_.c_str(), mdbUserPwd_.c_str(), mdbName_.c_str(), 0, NULL, 0))
	{
		cout << "lien: " << __LINE__ << "error connecting to database : " << mysql_error(conn_);
		//TRACE(LOG_ERROR, "Error connecting to database :" << mysql_error(conn_));
	}
	else
	{
		cout << "line: " << __LINE__ << "connected..." << endl;
		//TRACE(LOG_ERROR, "Connected...");
	}
	mysql_set_character_set(conn_, "utf8");

	return 0;
}

int Mysql::reConnectDB()
{
	//mysql_free_result(res);
	mysql_close(conn_);
	cout << "line: " << __LINE__ << "reConnectDB mysql....." << endl;
	//TRACE(LOG_ERROR, "reConnectDB mysql......");

	initDB();
}

int Mysql::insertMysql(VectorQuestion &question)
{
	stringstream sql;

	sql << "insert into t_test (stem)values('" 
		<< question.stem_
		<< "');";


	cout << "line: " << __LINE__ << "sql: " << sql.str() << endl;
	//TRACE(LOG_ERROR, "Mysql::Mysql, sql: " << sql.str());
	int count = 0;

	int t = 0;

	try
	{
		//t = mysql_query(conn_, sql.str().c_str());
		//
		t = mysql_real_query(conn_, sql.str().c_str(), sql.str().size());
		if(t)
		{
			cout << "line: " << __LINE__ << "dbcmd(dbport, sql.str().c_str()) error" << endl;
			//TRACE(LOG_ERROR, "Mysql::Mysql dbcmd(dbproc, sql.str().c_str()) error! ");
			reConnectDB();
		}
	} 
	catch (...)
	{
		cout << "line: " << __LINE__ << "database Mysql exception " << endl;
		//TRACE(LOG_ERROR, "database Mysql exception ");
		reConnectDB();
		return -3;
	}
	//TRACE(LOG_ERROR, "count : " << count);

	return 0;
}

int Mysql::insertStemMysql(const VectorQuestion &question)
{
	stringstream sql;

	sql << "insert into t_test (stem)values('" 
		<< question.stem_
		<< "');";


	cout << "line: " << __LINE__ << "   Stemsql: " << sql.str() << endl;
	//TRACE(LOG_ERROR, "Mysql::Mysql, sql: " << sql.str());
	int count = 0;

	int t = 0;

	try
	{
		//t = mysql_query(conn_, sql.str().c_str());
		//
		t = mysql_real_query(conn_, sql.str().c_str(), sql.str().size());
		if(t)
		{
			cout << "line: " << __LINE__ << "dbcmd(dbport, sql.str().c_str()) error" << endl;
			//TRACE(LOG_ERROR, "Mysql::Mysql dbcmd(dbproc, sql.str().c_str()) error! ");
			reConnectDB();
		}
	} 
	catch (...)
	{
		cout << "line: " << __LINE__ << "database Mysql exception " << endl;
		//TRACE(LOG_ERROR, "database Mysql exception ");
		reConnectDB();
		return -3;
	}
	//TRACE(LOG_ERROR, "count : " << count);

	return 0;
}

int Mysql::getStemId(VectorQuestion &question)
{
	stringstream sql;

	sql << "select stem_id from t_test where stem = '"
		<< question.stem_
		<< "';";

	cout << "line: " << __LINE__ << "  getStemIdsql: " << sql.str() << endl;

	MYSQL_RES *res;
	MYSQL_ROW row;

	int t = 0;

	try
	{
		t = mysql_real_query(conn_, sql.str().c_str(), sql.str().size());
		if (t)
		{
			cout << "lien: " << __LINE__ << "dbcmd(dbporc, sql_str().c_str()) error !!" << endl;
		}
		else
		{
			res = mysql_use_result(conn_);
			int rows = mysql_num_rows(res);
			if (res)
			{
				while ((row = mysql_fetch_row(res)) != NULL)
				{
					question.stem_id_ = atoi(row[0]);
				}
			}
			mysql_free_result(res);
		}
	}
	catch(...)
	{
		cout << "line: " << __LINE__ << "database getStemId exception !" << endl;
		reConnectDB();
		return -3;
	}

	return 0;
}


int Mysql::insertAnswerMysql(const VectorQuestion &question)
{
	stringstream sql;

	sql << "insert into t_test_answer (stem_id, answer)values(" 
		<< question.stem_id_
		<< ",'"
		<< question.answer_
		<< "');";


	cout << "line: " << __LINE__ << "   Answersql: " << sql.str() << endl;
	//TRACE(LOG_ERROR, "Mysql::Mysql, sql: " << sql.str());
	int count = 0;

	int t = 0;

	try
	{
		//t = mysql_query(conn_, sql.str().c_str());
		//
		t = mysql_real_query(conn_, sql.str().c_str(), sql.str().size());
		if(t)
		{
			cout << "line: " << __LINE__ << "dbcmd(dbport, sql.str().c_str()) error" << endl;
			//TRACE(LOG_ERROR, "Mysql::Mysql dbcmd(dbproc, sql.str().c_str()) error! ");
			reConnectDB();
		}
	} 
	catch (...)
	{
		cout << "line: " << __LINE__ << "database Mysql exception " << endl;
		//TRACE(LOG_ERROR, "database Mysql exception ");
		reConnectDB();
		return -3;
	}
	//TRACE(LOG_ERROR, "count : " << count);

	return 0;
}

int Mysql::insertOptionMysql(const VectorQuestion &question)
{
	stringstream sql;
	stringstream optionSs;
	string option = "";
	for (int i = 0; i < question.option_.size(); ++i)
	{
		optionSs << "(" << question.stem_id_ << ",'" << question.option_[i] << "'),";
	}
	option = optionSs.str().substr(0, optionSs.str().size() - 1);

	sql << "insert into t_test_option (stem_id, `option`)values" 
		<< option
		<< ";";


	cout << "line: " << __LINE__ << "   Optionsql: " << sql.str() << endl;
	//TRACE(LOG_ERROR, "Mysql::Mysql, sql: " << sql.str());
	int count = 0;

	int t = 0;

	try
	{
		//t = mysql_query(conn_, sql.str().c_str());
		//
		t = mysql_real_query(conn_, sql.str().c_str(), sql.str().size());
		if(t)
		{
			cout << "line: " << __LINE__ << "dbcmd(dbport, sql.str().c_str()) error" << endl;
			//TRACE(LOG_ERROR, "Mysql::Mysql dbcmd(dbproc, sql.str().c_str()) error! ");
			reConnectDB();
		}
	} 
	catch (...)
	{
		cout << "line: " << __LINE__ << "database Mysql exception " << endl;
		//TRACE(LOG_ERROR, "database Mysql exception ");
		reConnectDB();
		return -3;
	}
	//TRACE(LOG_ERROR, "count : " << count);

	return 0;
}	
