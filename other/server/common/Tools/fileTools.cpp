#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "fileTools.h"
#include "errorTools.h"
#include "../debugtrace.h"

using namespace std;

//构造函数
FileTools::FileTools(const char *filepath)
    :m_filepath(filepath)
{
    m_dirFd = NULL;
    //以读模式打开文件
    m_fd = fopen(m_filepath.c_str(), "r");
    if(m_fd == NULL)
    {
        m_errtool = strerror(errno); //获取系统异常信息
        throw(m_errtool); //抛出异常
    }
}

FileTools::FileTools():m_filepath(""),m_errtool("Not error")
{
    m_fd = NULL;
    m_dirFd = NULL;
}

//析构函数
FileTools::~FileTools(void)
{
    //关闭文件
    if(m_fd != NULL)
    {
        fclose(m_fd);
    }
}

//读文件
//参数：lines:读取多少行, 默认读取1行
bool FileTools::readNline(std::string &buf, int lines/*=1*/)
{
    if(m_fd == NULL)
    {
        m_errtool = "m_fd == NULL";
        throw(m_errtool);
    }
    char read_buf[BUFSIZE] = {0};
    buf.clear(); //清空buf
    while(fgets(read_buf,sizeof(read_buf), (m_fd))!= NULL)
    {
        buf += read_buf;
        if(!--lines)
            break;
    }

    if(lines > 0)
    {
        return false;
    }

    return true;
}

//写文件 暂时不实现
int FileTools::writeFile(std::string &buf)
{
    return 0;
}

//获取配置信息
//描述：每次获取时都从头获取
//注意：使用这个函数配置文件中不能有重复的配置项名称
bool FileTools::getConf(const char *title, std::string &data)
{
	std::string buf;
	char *str = NULL;
	char line[BUFSIZE] = {0}, ret[BUFSIZE] = {0};
	resetFd(); //重置读取位置
	while(readNline(buf))//读取一行
	{
		sprintf(line, "%s", buf.c_str()); 
		if((str = strstr(line, title)) != NULL) //查找关键字
		{
			buf = str;
			size_t pos = buf.find('\r');
			if(pos == string::npos)
			{
				pos = buf.find('\n');
			}
			data = buf.substr(0, pos);
			//line[strlen(line)-2] = '\0'; //截取字符串
			//sprintf(ret, "%s", (str + strlen(title) + 1)); 
			//data = (str + strlen(title) + 1); //返回读取到得配置内容
			return true;
		}
	}
	
	return false; //读取到文件尾没有找到数据
}

//重载读取配置信息
//描述：按组读取，组名不可相同，不同组的配置项名称可以重复
bool FileTools::getConf(const char *name, const char *title, std::string &data)
{
    std::string buf;
    size_t found = 0;
    resetFd(); //重置读取位置
    while(readNline(buf))//读取一行
    {
        if(string::npos != (found = buf.find(name)) ) //查找组名
        {
            buf.clear(); //清空buf
            while(readNline(buf)) //读取一行
            {
                //判断是否读取到配置组尾部
                if(string::npos != (found = buf.find_first_of('[')) )
                {
                    //读取完本组数据,没有找到
		    std::string errBuf;
		    errBuf = "没有找到配置成员: ";
		    errBuf += title;
                    m_errtool = errBuf.c_str();
                    throw(m_errtool);

                }
                if(string::npos != (found = buf.find(title)) ) //查找配置名
                {
                    //找到=的位置
                    if((found = buf.find('=')) != string::npos)
                    {
                        //截取字符串
                        data = buf.substr(found + 1);
			found = data.find('\r');
			if(found == string::npos)
			{
				found = data.find('\n');
			}
			data = data.substr(0, found);
                        return true;
                    }
                    else
                    {
                        //没有找到符号=
                        m_errtool = "配置文件格式错误，没有找到‘=’;";
                        throw(m_errtool); //抛出异常
                    }
                }
            }//查找组成员结束
        }
    }//查找组名结束
    //读取文件完毕没有找到组
    m_errtool = "没有这个这个配置组: ";
    m_errtool += name;
    throw(m_errtool);

    return false;
}

//重置读取位置
bool FileTools::resetFd(void)
{
    if(fseek(m_fd, 0, SEEK_SET) == -1) 
    {
        return false;
    }
    return true;
}

//获取文件名
bool FileTools::getFilename(const char *dirname, std::string &buf, int num/*=1*/)
{
    if((m_dirFd = opendir(dirname)) == NULL)
    {
        m_errtool = strerror(errno); //获取错误信息
        throw(m_errtool); //抛出异常
    }
    struct dirent *p_dir = NULL;
    while((p_dir = readdir(m_dirFd)) != NULL) //读取目录下的文件名
    {
        char tmp[200] = {0};
        if(strcmp(dirname+strlen(dirname)-1, "/") == 0)
        {
            sprintf(tmp, "%s%s",dirname, p_dir->d_name);
        }
        else
        {
            sprintf(tmp, "%s/%s", dirname, p_dir->d_name);
        }
        tmp[strlen(tmp)] = '\0';
        buf = tmp; //获取文件名
        if(!--num)
            break;
    }

    if(num > 0)
        return false;
    else
        return true;
}





