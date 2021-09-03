/*
 * 作者：刘洋
 * 日期：2016-11-15
 * 描述：对异常操作封装
 * 版本：1.0.0.0
 * 说明：高版本可以取代低版本，反之不可以。1.0.0.0 > 没有版本号
 * */
#ifndef ERRORTOOLS_H__
#define ERRORTOOLS_H__
#include <string>
#include <iostream>

class ErrorTools:public std::exception{
public:
    //构造函数
    ErrorTools(const char *text);
    ErrorTools(void);
    ~ErrorTools()throw();

    const char* what(void)const throw();

    ErrorTools& operator=(const char *)throw();
    ErrorTools& operator+=(const char *)throw();
public:
    int m_errCode; //错误代码
private:
    std::string     m_text; //错误信息
};
#endif //ERRORTOOLS_H__
