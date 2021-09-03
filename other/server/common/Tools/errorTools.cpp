#include "errorTools.h"
//构造函数
ErrorTools::ErrorTools(const char *text)
		:m_text(text),m_errCode(0) {}

ErrorTools::ErrorTools(void)
		:m_text("Not error data"),m_errCode(0){}

//析构函数
ErrorTools::~ErrorTools()throw()
{}

//返回错误信息
const char* ErrorTools::what(void)const throw()
{
    return m_text.c_str();
}

//异常内容的赋值
ErrorTools& ErrorTools::operator=(const char *str)throw()
{
    m_text = str;
    return *this;
}

ErrorTools& ErrorTools::operator+=(const char *str)throw()
{
	m_text += str;
	return *this;
}
