#ifndef _USERINFO_H
#define _USERINFO_H

#include <map>
#include <stdlib.h>
#include <string>

using namespace std;

class UserInfo
{
public:
  UserInfo();
  ~UserInfo();

  void setValue(const string& key, const string& value, const string& type);
  int64_t getValueInt64(const string& key);
  int32_t getValueInt32(const string& key);
  string getValueString(const string& key);
  bool getValueBool(const string& key);
  int32_t getValueBoolInt(const string& key);

  string getKeyType(const string& key);   //根据key,返回存储类型 string  int64_t int32_t bool
  //bool boolInt(const string& key);
private:
  map<string, string>       mapValueType_;     //记录值类型的map，类型为"string"  "int64_t"
  map<string, int64_t>      mapInt64Value_;      //保存int64_t类型的map
  map<string, string>       mapStringValue_;     //保存string类型的map
  map<string, int32_t>      mapInt32Value_;      //保存int32_t类型的map

  map<string, int32_t>      mapBoolValue_;       //使用int 保存bool类型的map 0,1
};

#endif // _USERINFO_H
