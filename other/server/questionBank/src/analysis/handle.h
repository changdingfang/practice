#pragma once
# include "mysql.h"
# include "analysis.h"

# include <string>

class Handle
{
public:
	Handle()	{	};
	~Handle()	{	};
	
	string		trim(string &);
	void		readDBConfig();
	int 		handle();

private:
	string		mip_;
	string		mport_;
	string		mdbName_;
	string		mdbUser_;
	string		mdbUserPwd_;

	//解析文件名
	string		readFileName_;

	//解析后数据写入文件名
	string		writeFileName_;

	Mysql			mysql_;
	ChoiceQuestion  cq_;
};

