/*
 * 作者：刘洋
 * 时间：2016-11-17
 * 说明：编码转换
 * */
#ifndef _SWAPCODE_H_
#define _SWAPCODE_H_

#include "errorTools.h"

#include <iconv.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <string>

class SwapCodeTools
{
public:
	SwapCodeTools(void);
	~SwapCodeTools(void);

	/*UNICODE码转为GB2312码*/
	std::string u2g(const std::string &inbuf);
	//std::string u2g(const char *inbuf, size_t lent);

	/*GB2312码转为UNICODE码*/
	std::string g2u(const std::string &inbuf);
	//std::string g2u(const char *inbuf, size_t len);

	/*UNICODE码转为GB2312码*/
	int u2g(char *inbuf, size_t inlen, char *outbuf, size_t outlen);

	/*GB2312码转为UNICODE码*/
	int g2u(char *inbuf, size_t inlen, char *outbuf, size_t outlen);

	//char类型转换到int类型
	char Char2Int(char ch);
	//string 类型转换为进制类型
	char Str2Bin(char *str);
	//char类型转换到16进制
	unsigned char CharToHex(const unsigned char &abyChValue);
	//url编码
	std::string UrlEncode(const std::string& astrData);
	//url解码
	std::string UrlDecode(const std::string& astrData);
	//转换HTTP危险字符
	static std::string FilterHTMLReservedCharacter(const char* p);
private:
	/*代码转换:从一种编码转为另一种编码*/
	int code_convert(const char *from_charset, const char *to_charset,
	char *inbuf, size_t inlen, char *outbuf, size_t outlen);

private:
	ErrorTools          m_errTool;      //异常函数
};

#endif //_SWAPCODE_H_
