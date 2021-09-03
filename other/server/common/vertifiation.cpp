#include "curl_http.h"
#include "vertifiation.h"
#include "debugtrace.h"
#include <iostream>

CVertifiation::CVertifiation(const string& key, const string& rule):result(false), md5key(key), rule_(rule)
{

}

CVertifiation::~CVertifiation()
{
	
}

int CVertifiation::Md5encrypt(const string& src, string& myToken)
{
  std::stringstream secret_ss;
  secret_ss << src;
  
  unsigned char secret_key_md5[16];
  memset(secret_key_md5, 0, 16);
  char secret_key_md5_str[34];
  memset(secret_key_md5_str, 0, 34);
  char* p = secret_key_md5_str;
  
  std::string secret_key_orig = secret_ss.str();
  MD5((const unsigned char*)secret_key_orig.c_str(), secret_key_orig.size(), secret_key_md5);
  
  for (int i = 0; i < 16; ++i) 
  {
    sprintf(p, "%02x", secret_key_md5[i]);
    p += 2;
  }
  
  myToken = secret_key_md5_str;
  
  return 0;
}

bool CVertifiation::verifySign(UserInfo& userInfo)
{
  string md5Value;
  stringstream t1;
  
  size_t nDotLeft = 0;
  size_t nDotRight = 0;
  string strKey;
  while ((nDotRight = rule_.find(',', nDotLeft)) != string::npos)
  {
    strKey = rule_.substr(nDotLeft, nDotRight - nDotLeft);
    nDotLeft = nDotRight + 1;
    if (strKey == "md5key") {
      t1 << md5key;
      continue;
    }

    string type = userInfo.getKeyType(strKey);
    if (type == "string") {
      t1 << userInfo.getValueString(strKey);
    } else if (type == "int64_t") {
      t1 << userInfo.getValueInt64(strKey);
    } else if (type == "int32_t") {
      t1 << userInfo.getValueInt32(strKey);
    } else if (type == "bool") {
      t1 << userInfo.getValueBoolInt(strKey);
    } else {
      TRACE(LOG_ERROR,"unknow type: " << type);
    }
  }

  TRACE(LOG_ERROR, "t1: " << t1.str());

  Md5encrypt(t1.str(), md5Value);
  
  TRACE(LOG_ERROR, "myMD5: " << md5Value << "  sign: " << userInfo.getValueString("sign"));
  
  if(md5Value.compare(userInfo.getValueString("sign")) != 0)
  {
    return false;
  }
  
  return true;
}

bool CVertifiation::verifyToken(UserInfo& userInfo, string url)
{
  string md5Value;
  stringstream t1;
  
  stringstream streamUrl;
  streamUrl << url << "?uid=" << userInfo.getValueInt64("uid") 
            << "&token=" << userInfo.getValueString("token");

  TRACE(5, "CVertifiation::verifyToken url:" << streamUrl.str());

  Data data;
  CHttp http;
  http.HttpGet(streamUrl.str().c_str(), data);

  int ret = 0;
  Json::Value jsonObj;
  std::stringstream iss((const char*)data.Buffer());
  try {
    iss >> jsonObj;
  }
  catch (std::exception& e) {
  	TRACE(5, "CVertifiation::verifyToken exception:" << e.what() << "    data.Buffer:" << data.Buffer());
    return true;
  }

  ret = jsonObj.get("code", -1).asInt();
  if (ret != 0) {
    TRACE(5, "CVertifiation::verifyToken error!");
    return false;
  } 

  return true;
}

int CVertifiation::hmac_sha1(const string& mydata, const string& secretKey, string& myResult)
{     
  unsigned int len = 20;
  unsigned char* result = (unsigned char*)malloc(sizeof(char) * len); //签名后数据  20位不可见字符
  memset(result, 0, len);
  if(NULL == result)
  {
    return -1;
  }
  
  HMAC_CTX ctx;
  HMAC_CTX_init(&ctx);
  HMAC_Init_ex(&ctx, secretKey.c_str(), secretKey.length(), EVP_sha1(), NULL);
  HMAC_Update(&ctx, (unsigned char*)mydata.c_str(), mydata.length());
  HMAC_Final(&ctx, result, &len);
  HMAC_CTX_cleanup(&ctx);
  
  char hmacSha1Str[64];       //签名后十六进制可见字符  40Byte
  memset(hmacSha1Str, 0, 64);
  char* p = hmacSha1Str;
  
  for (unsigned int i = 0; i < len; i++)
  {
    sprintf(p, "%02x", (unsigned int)result[i]);
    p += 2;
  }
  myResult = hmacSha1Str;
  
  if(NULL != result)
  {
    free(result);
    result = NULL;
  }
  p = NULL;
  
  return 0;
}

std::string CVertifiation::getFormattedText(unsigned char buf[], int len) 
{
  char HEX_DIGITS[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
  std::string callback;
  for (int j = 0; j < len; j++) {
    callback.append(1, HEX_DIGITS[(buf[j] >> 4) & 0x0f]);
    callback.append(1, HEX_DIGITS[buf[j] & 0x0f]);
  }

  return callback;
}

bool CVertifiation::SHA1(std::string in, std::string& out)
{
  SHA_CTX stx;
  unsigned char outmd[20] = {0};  //注意这里的字符个数为20
  int len = in.length();
 
  SHA1_Init( &stx );
  SHA1_Update(&stx, in.c_str(), len);
  SHA1_Final(outmd, &stx);
  
  out = CVertifiation::getFormattedText(outmd, 20);
  TRACE(5, "CVerifiation::SHA1  out: " << out);
}
