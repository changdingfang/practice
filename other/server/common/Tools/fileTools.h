/*
 * 作者：刘洋
 * 日期：2016-11-15
 * 描述：对异常操作封装
 * 版本：1.0.0.0
 * 说明：高版本可以取代低版本，反之不可以。1.0.0.0 > 没有版本号
 * */
#ifndef FILETOOLS_H__
#define FILETOOLS_H__

#define BUFSIZE 1024

#include <string>
#include <sys/types.h>
#include <dirent.h>

#include "errorTools.h"

class FileTools{
public:
    //构造函数
    FileTools(const char *filepath);

    FileTools();

    //析构函数
    ~FileTools(void);

    //读文件内容
    bool readNline(std::string &buf, int lines = 1);

    //获取配置信息
    //描述：每次获取时都从头获取
    //注意：使用这个函数配置文件中不能有重复的配置项名称
    bool getConf(const char *title, std::string &data);

    //重载读取配置信息
    //描述：按组读取，组名不可相同，不同组的配置项名称可以重复
    bool getConf(const char *name, const char *title, std::string &data);

    //获取log文件名
    bool getFilename(const char *dirname, std::string &buf, int num = 1);

    //重置读取位置
    bool resetFd(void);

    //写文件
    int writeFile(std::string &buf);

private:
    std::string     m_filepath; //操作文件
    ErrorTools      m_errtool;  //错误处理
    FILE*           m_fd; //文件操作符
    DIR*            m_dirFd; //文件夹操作符
};

#endif //FILETOOLS_H__
