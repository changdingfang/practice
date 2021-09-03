#ifndef CURL_HTTP_H
#define CURL_HTTP_H

#include <pthread.h>
#include <curl/curl.h>
#include <vector>
#include <string>
#include <stdint.h>
#include "data.h"

#define PROPS_RECV_BUF_LEN       204800


class CHttp
{
public:
  CHttp();
  ~CHttp();
  
public:
  int HttpGet(const char* url, Data& recv_data);
  int HttpGet_v2(const char* url, Data& recv_data, double& dns_time);
  int HttpPost(char* url, Data& content, Data& recv_data);
  int HttpPost(const char *url, const std::string& param, Data& recv_data);
  int HttpPost(const char* url, const char *buf, std::vector<std::string>& v_headers, Data& recv_data);
  int GetStatusCode();
  
private:
  static int RecvData(char* data, size_t size, size_t nmemb, Data* recv_data);

private:
  CURL* curl_handle_;      //curl handle
};

#endif   //CURL_HTTP_H
