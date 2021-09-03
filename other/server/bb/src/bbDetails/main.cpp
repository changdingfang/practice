#pragma once

#include "bbDetails.h"
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
	LogTools log_tools("bbDetails", 5);
	if(0 != log_tools.createLog())
	{
		return -1; 
	}

	TRACE(LOG_ERROR, "bbDetails tart");

	BbDetails bb;
	bb.readDBConfig();
	int ret = bb.initDB();
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
	string fcgiRetData;
	int fcgiRet = 0;

	string callback = "";

	while (FCGX_Accept(&fcgiStdIn, &fcgiStdOut, &fcgiStdErr, &fcgiParamArray) >= 0)
	{
		log_tools.rebuild(); ///每日重建日志
		fcgiRetData.clear();
		FCGX_FPrintF(fcgiStdOut, "%s\r\n", CONTENT_TYPE);
		fcgibuffer = FCGX_GetParam("REQUEST_METHOD", fcgiParamArray);
		fcgiRet = 0;
		do {
			if (0 != strcmp("GET", fcgibuffer))
			{
				TRACE(LOG_ERROR, "http method is not GET");
				fcgiRet = -100;
				break;
			}
			// param
			fcgibuffer = FCGX_GetParam("QUERY_STRING", fcgiParamArray);
			fcgiRetData = "";
			fcgiRet = bb.messageHandle(fcgibuffer, fcgiRetData, callback);
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
						msg = "Details error";
						break;
					}
			}

			Json::Value jsonValue;
			jsonValue["code"]   =  fcgiRet;
			jsonValue["msg"]    =  msg;
			Json::FastWriter fastWriter;
			fcgiRetData = fastWriter.write(jsonValue);
		}

		if (!callback.empty())
		{
			fcgiRetData = callback + "(" + fcgiRetData +")";
		}

		FCGX_FPrintF(fcgiStdOut, "%s", fcgiRetData.c_str());
		TRACE(LOG_TRACE, "deal http request:" << fcgibuffer << ", ret: " << fcgiRet << ", response: " << fcgiRetData);
	}

	return 0;
}
