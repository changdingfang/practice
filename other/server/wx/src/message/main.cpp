#pragma once

#include "message.h"
#include "common/debugtrace.h"
#include "common/log_tools.h"

#include <unistd.h>
#include <sstream>
#include <fstream>
#include <string>
#include <fcgiapp.h>
#include <iostream>

#define CONTENT_TYPE "Content-Type: text/json; charset=utf-8\r\n"

int main()
{
	LogTools log_tools("message", 5);
	if(0 != log_tools.createLog())
	{
		return -1; 
	}

	TRACE(LOG_ERROR, "message tart");

	Message message;
	message.readDBConfig();
	int ret = message.initDB();
	if(ret != 0)
	{
		TRACE(LOG_ERROR, "initDB() error");
		return -3;
	}

	const char*		fcgibuffer = NULL;
	FCGX_Stream*	fcgiStdIn  = NULL;
	FCGX_Stream*	fcgiStdOut = NULL;
	FCGX_Stream*	fcgiStdErr = NULL;
	FCGX_ParamArray	fcgiParamArray;
	string	fcgiRetData;
	string	receiveData;
	int fcgiRet = 0;

	while (FCGX_Accept(&fcgiStdIn, &fcgiStdOut, &fcgiStdErr, &fcgiParamArray) >= 0)
	{
		log_tools.rebuild(); ///每日重建日志
		fcgiRetData.clear();
		FCGX_FPrintF(fcgiStdOut, "%s\r\n", CONTENT_TYPE);
		fcgibuffer = FCGX_GetParam("REQUEST_METHOD", fcgiParamArray);
		fcgiRet = 0;
		do {
			if (0 == strcmp("GET", fcgibuffer))
			{
				TRACE(LOG_ERROR, "http method is GET");
				fcgibuffer = FCGX_GetParam("QUERY_STRING", fcgiParamArray);
				fcgiRetData = "";
				fcgiRet = message.messageHandle(fcgibuffer, fcgiRetData);

			}
			else if (0 == strcmp("POST", fcgibuffer))
			{
				int length = 0;
				char *plen = FCGX_GetParam("CONTENT_LENGTH", fcgiParamArray);
				if (NULL != plen)
				{
					length = atoi(plen);
					TRACE(LOG_TRACE, "length: " << length);
				}
				char *data = (char *)malloc(length + 1);//申请内存, 存放数据 
				memset(data, 0, length + 1);
				int fcgilen = FCGX_GetStr(data, length, fcgiStdIn);
				receiveData = data;
				TRACE(LOG_ERROR, "data: " << receiveData);
				if (NULL != data)
				{
					free (data);
					data = NULL;
				}
				fcgiRetData = "";
				fcgiRet = message.postHandle(receiveData, fcgiRetData);
			}
			// param
		} while (0);

		if (fcgiRet != 0)
		{
			string msg = "";

			switch (fcgiRet)
			{
				case -1:
					{
						msg = "parse_param_error";
						break;
					}
				case -2: 
					{
						msg = "sign_verify_error";
						break;
					}
				case -1000:
					{
						msg = "token_verify_error";
						break;
					}
				case -4:
					{
						msg = "getFollowPage error";
						break;
					}
			}

			Json::Value jsonValue;
			jsonValue["code"]   =  fcgiRet;
			jsonValue["msg"]    =  msg;
			Json::FastWriter fastWriter;
			fcgiRetData = fastWriter.write(jsonValue);
		}

		FCGX_FPrintF(fcgiStdOut, "%s", fcgiRetData.c_str());
		TRACE(LOG_TRACE, "deal http request:" << fcgibuffer << ", ret: " << fcgiRet << ", response: " << fcgiRetData);
	}

	return 0;
}
