 //***************************************************************
 // @file:    base64.h
 // @author:  dingfang
 // @date    2018-09-26 20:39:20
 //***************************************************************
 
#include <string>
 
std::string base64_encode(unsigned char const* , unsigned int len);
std::string base64_decode(std::string const& s);

