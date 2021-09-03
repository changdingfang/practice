#include "mongodbTools.h"
#include <string>

using namespace std;
using namespace mongo; //mongo命名空间

//构造和析构函数
MongodbTools::MongodbTools(void):
            m_mongo(true, 0, 5), m_errTool(""),m_connStat(false)
{
}

MongodbTools::~MongodbTools(void)
{
}

//初始化,与mongodb建立连接
void MongodbTools::init(const std::string host, const std::string dbname)
{
    m_sName = dbname; //赋值数据库名
    m_sHost = host; 
    string msg;
    //连接数据库
    m_mongo.connect(host, msg);
    m_connStat = true; //连接成功，修改连接状态
}

//执行插入操作
int MongodbTools::insertMongo(const mongo::BSONObj &obj, int flags /*= 0*/)
{
    //判断是否连接MongoDB
    if(!m_connStat)
    {
        	m_errTool = "未初始化MongoDB!";
        	throw(m_errTool);
    }

    //执行插入操作
    m_mongo.insert(m_sName, obj, flags);
    return 0;
}

//更新操作
int MongodbTools::updateMongo(const mongo::Query &query, mongo::BSONObj obj, bool upsert/*= false*/, bool multi /*= false*/)
{
	if(!m_connStat)
	{
        	m_errTool = "未初始化MongoDB!";
        	throw(m_errTool);
	}

	//执行更新操作
	m_mongo.update(m_sName, query, obj, upsert, multi);
	return 0;
}

//执行读取操作
//query: mongo查询条件
//p_handle: 对查到的数据卓条进行操作的回调函数
//json_value: 查到的结果,Json串
int MongodbTools::selectMongo(const mongo::Query &query, HandleBson p_handle, Json::Value &json_value,
                                    int ret/* = 0*/, int skip/* = 0*/)
{
    //判断是否连接MongoDB
    if(!m_connStat)
    {
        m_errTool = "未初始化MongoDB!";
        throw(m_errTool);
    }

    //查询mongodb
    auto_ptr<mongo::DBClientCursor> p_corsr = m_mongo.query(m_sName, query, ret, skip);

    int cnt = 0;
    if(!p_corsr.get())
    {
    	return 0;
    }
    while(p_corsr->more())
    {
        Json::Value value;
        mongo::BSONObj curr = p_corsr->next();
        p_handle(curr, value); //调用回调函数处理查询结果
	if(!value.empty())
	{
        	json_value.append(value); //添加数据
		cnt++;
	}
    }
    
    return cnt; //返回查询到的数量
}

//执行读取操作
//query: mongo查询条件
//p_handle: 对查到的数据进行操作的回调函数
//json_value: 查到的结果,Json串
//other: 用于扩展
int MongodbTools::selectMongo(const mongo::Query &query, HandleBsons p_handle, Json::Value &json_value,
                                    int ret/* = 0*/, int skip/* = 0*/, void* other)
{
    //判断是否连接MongoDB
    if(!m_connStat)
    {
        m_errTool = "未初始化MongoDB!";
        throw(m_errTool);
    }

    //查询mongodb
    auto_ptr<mongo::DBClientCursor> p_corsr = m_mongo.query(m_sName, query, ret, skip);

    int cnt = 0;
    if(!p_corsr.get())
    {
    	return cnt;
    }
    //for(cnt = 0; p_corsr->more(); cnt++)
    while(p_corsr->more())
    {
        Json::Value value;
        mongo::BSONObj curr = p_corsr->next();
        p_handle(curr, value, other); //调用回调函数处理查询结果
	if(!value.empty())
	{
        	json_value.append(value); //添加数据
		cnt++;
	}
    }
    
    return cnt; //返回查询到的数量
}


//执行mongodb计算操作
uint64_t MongodbTools::Count(mongo::BSONObjBuilder &obj, int option /*= 0*/, int limit /*= 0*/, int skip /*= 0*/)
{
	uint64_t cnt = 0;
	//判断是否连接MongoDB
	if(!m_connStat)
	{
		m_errTool = "未初始化MongoDB!";
		throw(m_errTool);
	}

	cnt = m_mongo.count(m_sName, obj.obj(), option, limit, skip);

	return cnt;
}


mongo::DBClientConnection* MongodbTools::get_mongo(void)
{
	if(!m_connStat)
	{
		m_errTool = "未初始化MongoDB!";
		throw(m_errTool);
	}
	return &m_mongo;
}




