# include "message.h"
# include "common/log_tools.h"

# include <time.h>

# include <iostream>
# include <fstream>
# include <algorithm>

//微信验证服务器的token
#define WX_TOKEN	"aly_token_naogua"

UserInfo::UserInfo()
{
}

UserInfo::~UserInfo()
{

}

Message::Message() : mip_(""), mport_("")
							   , mdbName_(""), mdbUser_("")
							   , mdbUserPwd_("") 
							   , conn_(NULL)
{

}

Message::~Message()
{
	if (NULL != conn_)
	{
		mysql_close(conn_);
		conn_ = NULL;
	}
}

int Message::messageHandle(const string &request, string &response)
{
	clean();
	int ret = parseQuery(request);
	if (0 != ret)
	{
		TRACE(LOG_ERROR, "parseQuery error, ret: " << ret << ", request: " << request);
		return -1;
	}

	vecToSign_.push_back(userInfo_.timestamp_);
	vecToSign_.push_back(userInfo_.nonce_);
	vecToSign_.push_back(userInfo_.token_);

	sort(vecToSign_.begin(), vecToSign_.end());

	//进行sh1加密生成signature
	sh1();

	if (mySign_ == userInfo_.signature_)
	{
		response = userInfo_.echostr_;
	}
	else
	{
		response = "wx test error!";
	}

# if 0
	Json::Value jsonValue;
	jsonValue["code"]      = 0;
	jsonValue["msg"]       = "success";
	if (mySign_ == userInfo_.signature_)
	{
		jsonValue["info"]	   = userInfo_.echostr_;
	}
	else
	{
		jsonValue["info"]	   = "wx test error!";
	}
	Json::FastWriter fastWriter;
	response = fastWriter.write(jsonValue);
# endif

	return 0;
}

int Message::sh1()
{
	stringstream ssTmp;
	ssTmp << vecToSign_[0] << vecToSign_[1] << vecToSign_[2];
	SHA1(ssTmp.str(), mySign_);

	return 0;
}

string Message::trim(string& s)
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

void Message::readDBConfig()
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

int Message::initDB()
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

int Message::reConnectDB()
{
	//mysql_free_result(res);
	mysql_close(conn_);
	TRACE(LOG_ERROR, "reConnectDB mysql......");

	initDB();
}

void Message::getParameter(const string &keyWord, const string &query, string &reply)
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

int Message::parseQuery(const string &query)
{
	string buf;

	//获取signature
	getParameter("signature=", query, buf);
	if (buf.empty())
	{
		TRACE(LOG_ERROR, "signature is empty");
		return -5;
	}
	userInfo_.signature_ = buf.c_str();
	buf.clear();

	//获取timestamp
	getParameter("timestamp=", query, buf);
	if (buf.empty())
	{
		TRACE(LOG_ERROR, "timestamp is empty");
		return -5;
	}
	userInfo_.timestamp_ = buf.c_str();
	buf.clear();

	getParameter("nonce=", query, buf);
	if (buf.empty())
	{
		TRACE(LOG_ERROR, "nonce is empty");
		return -5;
	}
	userInfo_.nonce_ = buf.c_str();
	buf.clear();

	getParameter("echostr=", query, buf);
	if (buf.empty())
	{
		TRACE(LOG_ERROR, "echostr is empty");
		return -5;
	}
	userInfo_.echostr_ = buf.c_str();
	buf.clear();

	userInfo_.token_ = WX_TOKEN;

	TRACE(LOG_TRACE, "Message::parseQuery signature: " << userInfo_.signature_
						<< "   timestamp: "	<< userInfo_.timestamp_
						<< "   nonce: "		<< userInfo_.nonce_
						<< "   echostr: "	<< userInfo_.echostr_
						<< "   token: "		<< userInfo_.token_);
}

int Message::getStem()
{
	stringstream sql;

	sql << "select stem_id, stem from t_test where stem_id between "
		<< " and "
		<< ";";

	TRACE(LOG_ERROR, "Message::Message, sql: " << sql.str());

	MYSQL_RES *res;
	MYSQL_ROW row;

	int t = 0;

	stringstream stemIdSs;

	try
	{
		t = mysql_real_query(conn_, sql.str().c_str(), sql.str().size());
		if(t)
		{
			TRACE(LOG_ERROR, "Message::Message dbcmd(dbproc, sql.str().c_str()) error! ");
			reConnectDB();
		}
		else
		{
			res=mysql_use_result(conn_);
			if(res)
			{
				while((row=mysql_fetch_row(res))!=NULL)
				{
					{
						Json::Value jsonTmp;
						jsonTmp["stem_id"]	= atoi(row[0]);
						jsonTmp["stem"]		= row[1];

						stemIdSs << atoi(row[0]) << ",";
					}
				}
			}
			mysql_free_result(res);
		}
	} 
	catch (...)
	{
		TRACE(LOG_ERROR, "database Message exception ");
		reConnectDB();
		return -3;
	}

	return 0;
}

void Message::clean()
{

	vecToSign_.clear();
	mySign_.clear();
	//userInfo_.cleanUserInfo();
}

bool Message::SHA1(std::string in, std::string& out)
{
	SHA_CTX stx;
	unsigned char outmd[20] = {0};  //注意这里的字符个数为20
	int len = in.length();

	SHA1_Init( &stx );
	SHA1_Update(&stx, in.c_str(), len);
	SHA1_Final(outmd, &stx);

	out = getFormattedText(outmd, 20);
	TRACE(5, "CVerifiation::SHA1  out: " << out);
}

string Message::getFormattedText(unsigned char buf[], int len)
{
	char HEX_DIGITS[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
	std::string callback;
	for (int j = 0; j < len; j++) {
		callback.append(1, HEX_DIGITS[(buf[j] >> 4) & 0x0f]);
		callback.append(1, HEX_DIGITS[buf[j] & 0x0f]);
	}

	return callback;
}

# if 0
<xml> 
<ToUserName>< ![CDATA[toUser] ]></ToUserName>
<FromUserName>< ![CDATA[fromUser] ]></FromUserName> 
<CreateTime>12345678</CreateTime> 
<MsgType>< ![CDATA[text] ]></MsgType> 
<Content>< ![CDATA[你好] ]></Content> 
</xml>
# endif

int Message::postHandle(const string &xml, string &response)
{
	size_t pos = 0;
	XmlParam xmlObject;
	xmlObject.find_str(xml, pos, xmlObject.toUserName_);
	xmlObject.find_str(xml, pos, xmlObject.fromUserName_);
	xmlObject.find_str(xml, pos, xmlObject.msgType_);
	xmlObject.find_str(xml, pos, xmlObject.content_);

	//xmlObject.print();

	stringstream ssData;
	string toUserName = xmlObject.fromUserName_;
	string fromUserName = xmlObject.toUserName_;
	string msgType = xmlObject.msgType_;
	string content = xmlObject.content_;
	uint64_t stamp = time(NULL);
	ssData << "<xml><ToUserName><![CDATA["
		   << toUserName 
		   << "]]></ToUserName><FromUserName><![CDATA["
		   << fromUserName
		   << "]]></FromUserName><CreateTime>"
		   << stamp
		   << "</CreateTime><MsgType><![CDATA["
		   << msgType
		   << "]]></MsgType><Content><![CDATA["
		   << content
		   << "]]></Content></xml>";
	
	response = ssData.str();

	return 0;
}

bool XmlParam::find_str(const string &param, size_t &pos, string &out)
{
	size_t posBeg = 0;
	size_t posEnd = 0;

	bool flag = false;

	pos = param.find("CDATA", pos);
	if ('[' == param[pos + 5])
	{
		posBeg = pos + 6;
		posEnd = param.find_first_of(']', posBeg);
		if (posEnd != string::npos)
		{
			out = param.substr(posBeg, posEnd - posBeg);
			flag = true;
		}
	}
	pos = posEnd;

	return flag;
}
