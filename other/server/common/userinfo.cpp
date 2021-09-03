#include "userinfo.h"

UserInfo::UserInfo() {

}

UserInfo::~UserInfo() {

}

//按照type类型不同，放入不同map中，暂时有string,int64_t,int32_t,bool
void UserInfo::setValue(const string& key, const string& value, const string& type)
{
  if (type == "string") {
    mapValueType_[key] = "string";
    mapStringValue_[key] = value;
  } else if (type == "int64_t") {
    mapValueType_[key] = "int64_t";
    mapInt64Value_[key] = atoll(value.c_str());
  } else if (type == "int32_t") {
    mapValueType_[key] = "int32_t";
    mapInt32Value_[key] = atoi(value.c_str());
  } else if (type == "bool") {
    mapValueType_[key] = "bool";
    mapBoolValue_[key] = atoi(value.c_str());
  }
}

int64_t UserInfo::getValueInt64(const string& key) {
  return mapInt64Value_[key];
}

int32_t UserInfo::getValueInt32(const string& key) {
  return mapInt32Value_[key];
}

string UserInfo::getValueString(const string& key) {
  return mapStringValue_[key];
}

int32_t UserInfo::getValueBoolInt(const string& key) {
  return mapBoolValue_[key];
}

bool UserInfo::getValueBool(const string& key) {
  if (getValueBoolInt(key) == 1) {
    return true;
  } else {
    return false;
  }
}

string UserInfo::getKeyType(const string& key) {
  return mapValueType_[key];
}
