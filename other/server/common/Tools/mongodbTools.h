#ifndef _MONGODBTOOLS_H__
#define _MONGODBTOOLS_H__

#include <string>
#include "mongo/client/dbclient.h"
#include "../json/json.h"

#include "errorTools.h"

typedef void (*HandleBson)(mongo::BSONObj &, Json::Value &);
typedef void (*HandleBsons)(mongo::BSONObj &, Json::Value &, void*);

class MongodbTools
{
public:
    //构造和析构函数
    MongodbTools(void);
    ~MongodbTools(void);

    //初始化,与mongodb建立连接
    //void init(const std::string &config);
    void init(const std::string host, const std::string dbname);

    //执行插入操作
    int insertMongo(const mongo::BSONObj &obj, int flags = 0);

    //更新操作
    int updateMongo(const mongo::Query &query, mongo::BSONObj obj, bool upsert = false, bool multi = false);

    //执行读取操作
    //query: mongo查询条件
    //p_handle: 对查到的数据进行操作的回调函数
    //json_value: 查到的结果,Json串
    int selectMongo(const mongo::Query &query, HandleBson p_handle, Json::Value &json_value,
                                    int ret = 0, int skip = 0);

    //执行读取操作
    //query: mongo查询条件
    //p_handle: 对查到的数据进行操作的回调函数
    //json_value: 查到的结果,Json串
    //other: 用于扩展
    int selectMongo(const mongo::Query &query, HandleBsons p_handle, Json::Value &json_value,
                                    int ret = 0, int skip = 0, void *other = NULL);
    //执行mongodb计算操作
    uint64_t Count(mongo::BSONObjBuilder &obj, int option = 0, int limit = 0, int skip = 0);

    mongo::DBClientConnection* get_mongo(void);

    //int selectMongo(const mongo::query, int ret = 0, int skip = 0);
private:
    //读取配置文件
    //void readConfig(const std::string &file);
private:
    mongo::DBClientConnection       m_mongo;        //数据库
    ErrorTools                      m_errTool;      //异常
    bool                            m_connStat;     //数据库连接状态
    std::string                     m_sHost;        //数据库地址
    std::string                     m_sName;        //数据库名
};

#endif //_MONGODBTOOLS_H__
