# include "handle.h"

string Handle::trim(string& s)
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

void Handle::readDBConfig()
{
	ifstream is_file("analysis.conf");
	if (!is_file.is_open())
	{
		cout << "line: " << __LINE__ << "Cannot find config: analysis.conf";
		//TRACE(LOG_ERROR,  "Cannot find config: analysis.conf" );
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
				else if (key == "analysis_file_name")
				{
					this->readFileName_ = trim(value);
				}
				else if (key == "analysis_after_file_name")
				{
					this->writeFileName_ = trim(value);
				}
			}
		}
	}
}

int Handle::handle()
{
	//读取配置
	readDBConfig();

	//初始化数据库连接数据库
	mysql_.init(mip_, mport_, mdbName_, mdbUser_, mdbUserPwd_);

	//初始化
	cq_.init(readFileName_, writeFileName_);

	//执行解析
	cq_.execute();

	//解析后数据写入文件
	cq_.writeFile();

	//打印解析后的数据
	//cq_.printQuestion();

	vector<VectorQuestion> vecQ = cq_.getVecQ();
	//插入到数据库
	for (int i = 1; i < vecQ.size(); ++i)
	{
		mysql_.insertStemMysql(vecQ[i]);
		mysql_.getStemId(vecQ[i]);
		mysql_.insertAnswerMysql(vecQ[i]);
		mysql_.insertOptionMysql(vecQ[i]);
	}
}
