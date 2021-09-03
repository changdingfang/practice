#pragma once

#include <unistd.h>
#include <string>
#include <sstream>
#include <openssl/md5.h>
#include <openssl/hmac.h>
#include <openssl/sha.h>
#include "userinfo.h"
#include "json/json.h"

using namespace std;

class CVertifiation
{
public:
  CVertifiation(const string& md5key, const string& rule);
  ~CVertifiation();
	
  int  Md5encrypt(const string& src, string& myToken);
  static int hmac_sha1(const string& mydata, const string& secretKey, string& myResult); //签名数据
	
  bool verifySign(UserInfo& userInfo);
  
  static bool verifyToken(UserInfo& userInfo, string url);
  static bool SHA1(std::string in, std::string& out);

private:
  static std::string getFormattedText(unsigned char buf[], int len);
	
public:
  bool     result;
  string   md5key;
  string   rule_;    //生成sign验证的规则字符串，使用,分隔
};
