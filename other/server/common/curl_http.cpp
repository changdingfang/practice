
#include <string>
using namespace std;

#include "curl_http.h"


CHttp::CHttp(void)
{
  curl_handle_ = NULL;

  curl_handle_ = curl_easy_init();
  if (curl_handle_) {
    //设置超时有bug, 暂时取消设置超时,但是有可能会导致线程阻塞
    //curl_easy_setopt(curl_handle_, CURLOPT_CONNECTTIMEOUT, 3);		//连接超时(秒)
    
    curl_easy_setopt(curl_handle_, CURLOPT_WRITEFUNCTION, CHttp::RecvData);
    curl_easy_setopt(curl_handle_, CURLOPT_NOSIGNAL, 1);
  }
}

CHttp::~CHttp(void)
{
  if (curl_handle_) {
    curl_easy_cleanup(curl_handle_);
    curl_handle_ = NULL;
  }
}

//get 方式
int CHttp::HttpGet(const char *url, Data& recv_data)
{
  if(NULL == url) {
    return -1;
  }
  
  try
  {
    if (curl_handle_) {
      curl_easy_setopt(curl_handle_, CURLOPT_URL, url);
      curl_easy_setopt(curl_handle_, CURLOPT_WRITEDATA, &recv_data);
      curl_easy_setopt(curl_handle_, CURLOPT_TIMEOUT, 5);
      curl_easy_setopt(curl_handle_, CURLOPT_CONNECTTIMEOUT, 3);
      curl_easy_setopt(curl_handle_, CURLOPT_NOSIGNAL, 1);
      if (CURLE_OK != curl_easy_perform(curl_handle_)) {
        return -1;
      }
    } else {
      return -1;
    }
  }
  catch(...)
  {
    return -1;
  }
  
  return 0;
}

int CHttp::HttpGet_v2(const char *url, Data& recv_data, double& dns_time)
{
  if(NULL == url) {
    return -1;
  }
 
  CURLcode res;
  try
  {
    if (curl_handle_) {
      curl_easy_setopt(curl_handle_, CURLOPT_URL, url);
      curl_easy_setopt(curl_handle_, CURLOPT_WRITEDATA, &recv_data);
      curl_easy_setopt(curl_handle_, CURLOPT_TIMEOUT, 5);
      curl_easy_setopt(curl_handle_, CURLOPT_CONNECTTIMEOUT, 3);
      curl_easy_setopt(curl_handle_, CURLOPT_NOSIGNAL, 1);
      res = curl_easy_perform(curl_handle_);
      curl_easy_getinfo(curl_handle_, CURLINFO_TOTAL_TIME, &dns_time);
    } else {
      return -2;
    }
  }
  catch(...)
  {
    return -3; 
  }
  
  return res;
}

/*
* content = "app=request&version=beta";
* HttpPost("http://xxx.xxx.xxx",content,recv_buf);
*/
int CHttp::HttpPost(char *url, Data& content, Data& recv_data)
{
  if (NULL == url) {
    return -1;
  }

  try
  {
    if (curl_handle_) {
      curl_easy_setopt(curl_handle_, CURLOPT_URL, url);
      curl_easy_setopt(curl_handle_, CURLOPT_POSTFIELDS, content.Buffer());  //参数
      curl_easy_setopt(curl_handle_, CURLOPT_WRITEDATA, &recv_data);
      curl_easy_setopt(curl_handle_, CURLOPT_TIMEOUT, 5);
      curl_easy_setopt(curl_handle_, CURLOPT_CONNECTTIMEOUT, 3);
      curl_easy_setopt(curl_handle_, CURLOPT_NOSIGNAL, 1);
      if ( CURLE_OK != curl_easy_perform(curl_handle_)) {
        return -1;
      }
    } else {
      return -1;
    }
  }
  catch(...)
  {
    return -1;
  }

  return 0;
}


/*
* param = "app=request&version=beta";
* HttpPost("http://xxx.xxx.xxx",param,recv_buf);
*/
int CHttp::HttpPost(const char *url, const string& param, Data& recv_data)
{
  if (NULL == url) {
    return -1;
  }

  try
  {
    if (curl_handle_) 
    {
      curl_easy_setopt(curl_handle_, CURLOPT_URL, url);
      curl_easy_setopt(curl_handle_, CURLOPT_POSTFIELDS, param.c_str());  //参数
      curl_easy_setopt(curl_handle_, CURLOPT_WRITEDATA, &recv_data);
      curl_easy_setopt(curl_handle_, CURLOPT_TIMEOUT, 5);
      curl_easy_setopt(curl_handle_, CURLOPT_CONNECTTIMEOUT, 3);
      curl_easy_setopt(curl_handle_, CURLOPT_NOSIGNAL, 1);
      if ( CURLE_OK != curl_easy_perform(curl_handle_)) 
      {
        return -1;
      }
    }else 
    {
      return -1;
    }
  }
  catch(...)
  {
    return -1;
  }

  return 0;
}

int CHttp::HttpPost(const char* url, const char *buf, std::vector<std::string>& v_headers, Data& recv_data)
{
  if (NULL == url) {
    return -1;
  }

  int res = 0;
  try
  {
    if (curl_handle_) {
      curl_easy_setopt(curl_handle_, CURLOPT_URL, url);
      struct curl_slist* headers = NULL;
	  for (int i=0; i<v_headers.size(); i++) {
        headers = curl_slist_append(headers, v_headers[i].c_str());
      }
	  if (headers != NULL) {
        //设置HTTP头  
        curl_easy_setopt(curl_handle_, CURLOPT_HTTPHEADER, headers);        
      }

      curl_easy_setopt(curl_handle_, CURLOPT_POSTFIELDS, buf);  //参数
      curl_easy_setopt(curl_handle_, CURLOPT_WRITEDATA, &recv_data);
      curl_easy_setopt(curl_handle_, CURLOPT_TIMEOUT, 5);
      curl_easy_setopt(curl_handle_, CURLOPT_CONNECTTIMEOUT, 3);
      curl_easy_setopt(curl_handle_, CURLOPT_NOSIGNAL, 1);
      if ( CURLE_OK != curl_easy_perform(curl_handle_)) {
        res = -1;
      }
	  if (headers != NULL) {
        curl_slist_free_all(headers);
        headers = NULL;
      }
    } else {
      res = -1;
    }
  }
  catch(...)
  {
    res = -1;
  }

  return res;
}

//call back
int CHttp::RecvData(char *data, size_t size, size_t nmemb, Data* recv_data)
{
  int len = size*nmemb;
  (len > PROPS_RECV_BUF_LEN-1) ? len = PROPS_RECV_BUF_LEN-1 : len;

  recv_data->Append((const uint8_t*)data, len);

  return len;
}

//return code
int CHttp::GetStatusCode()
{
  if (!curl_handle_)
    return -1;

  long   retcode = 0;
  if ( CURLE_OK != curl_easy_getinfo(curl_handle_, CURLINFO_RESPONSE_CODE, &retcode)) { 
    return -1;
  }

  return 0;
}

