#include "swapCodeTools.h"
#include "../debugtrace.h"
#include <errno.h>
#include <iostream>
#include <new>

using namespace std;

SwapCodeTools::SwapCodeTools(void){}
SwapCodeTools::~SwapCodeTools(void){}

/*代码转换:从一种编码转为另一种编码*/
int SwapCodeTools::code_convert(const char *from_charset, const char *to_charset,
                            char *inbuf, size_t inlen, char *outbuf, size_t outlen)
{
    iconv_t cd;
    char **pin = &inbuf;
    char **pout = &outbuf;
    
    cd = iconv_open(to_charset, from_charset);
    if (cd == 0)
    {
        TRACE(LOG_DEBUG, "iconv_open: " << strerror(errno) );
        return -1;
    }

    memset(outbuf, 0, outlen);
    if (iconv(cd, pin, &inlen, pout, &outlen) == (size_t)-1) 
    {
        TRACE(LOG_DEBUG, "iconv: " << strerror(errno) );
        return -2;
    }
    iconv_close(cd);

    return 1;
}

/*UNICODE码转为GB2312码*/
int SwapCodeTools::u2g(char *inbuf, size_t inlen, char *outbuf, size_t outlen)
{
    return code_convert("utf-8", "gb2312//TRANSLIT", inbuf, inlen, outbuf, outlen);
}

/*GB2312码转为UNICODE码*/
int SwapCodeTools::g2u(char *inbuf, size_t inlen, char *outbuf, size_t outlen)
{
    return code_convert("gbk", "utf-8//IGNORE", inbuf, inlen, outbuf, outlen);
}

/*UNICODE码转为GB2312码*/
//std::string SwapCodeTools::u2g(const char *inbuf, size_t len)
std::string SwapCodeTools::u2g(const std::string &inbuf)
{
    char *outbuf = new char[inbuf.length() + 50];
    if(outbuf == NULL)
    {
        m_errTool = "动态申请内存失败！";
        throw(m_errTool);
    }
    memset(outbuf, 0, inbuf.length() + 50);
    int cnt;
    if( (cnt = u2g((char*)inbuf.c_str(), inbuf.length(), outbuf, inbuf.length() )) < 0)
    {
        m_errTool = "u2g 转码失败！";
        throw(m_errTool);
    }
    outbuf[strlen(outbuf)]= '\0';
    string ret(outbuf);

    //TRACE(LOG_DEBUG, "\nret: " << ret << ";");

    delete[] outbuf;
    return ret;
}

/*GB2312码转为UNICODE码*/
//std::string SwapCodeTools::g2u(const char *inbuf, size_t len)
std::string SwapCodeTools::g2u(const std::string &inbuf)
{
    char *outbuf = new char[inbuf.length()];
    if(outbuf == NULL)
    {
        m_errTool = "动态申请内存失败！";
        throw(m_errTool);
    }

    memset(outbuf, 0, inbuf.length() + 50);
    int cnt = 0;
    if((cnt = g2u((char*)inbuf.c_str(), inbuf.length(), outbuf, inbuf.length() )) < 0)
    {
        //TRACE(LOG_DEBUG, "cnt: " << cnt);
        m_errTool = "g2u 转码失败！";
        throw(m_errTool);
    }
    outbuf[strlen(outbuf)]= '\0';
    string ret(outbuf);

    delete[] outbuf;
    return ret;
}

//char类型转换到int类型
char SwapCodeTools::Char2Int(char ch){
    if(ch>='0' && ch<='9')return (char)(ch-'0');
    if(ch>='a' && ch<='f')return (char)(ch-'a'+10);
    if(ch>='A' && ch<='F')return (char)(ch-'A'+10);
    return -1;
}

//string 类型转换为进制类型
char SwapCodeTools::Str2Bin(char *str){
    char tempWord[2];
    char chn;
    
    
    tempWord[0] = Char2Int(str[0]);                                //make the B to 11 -- 00001011
    tempWord[1] = Char2Int(str[1]);                                //make the 0 to 0  -- 00000000
    
    
    chn = (tempWord[0] << 4) | tempWord[1];                //to change the BO to 10110000
    
    
    return chn;
}

//char类型转换到16进制
unsigned char SwapCodeTools::CharToHex(const unsigned char &abyChValue)
{
    if(abyChValue > 9)
    {
        return abyChValue + ('A' - 10);
    }
    else
    {
        return abyChValue + '0';
    }
}

//url解码
string SwapCodeTools::UrlDecode(const string& astrData)
{
    string output="";
    char tmp[2];
    int i=0,len=(int)astrData.length();
    
    while(i<len){
        if(astrData[i]=='%'){
            tmp[0]=astrData[i+1];
            tmp[1]=astrData[i+2];
            output+=Str2Bin(tmp);
            i=i+3;
        }
        else if(astrData[i]=='+'){
            output+=' ';
            i++;
        }
        else{
            output+=astrData[i];
            i++;
        }
    }
    
    return output;
}

//url编码
string SwapCodeTools::UrlEncode(const string& astrData)
{
    std::string lstrData;
    unsigned char ch = 0;
    const char* pszSrc = astrData.c_str();
    for (int i = 0; i < (int)astrData.size(); i++)
    {
        ch = pszSrc[i];
        
        if ( (ch >= '0' && ch <='9')
            || (ch >= 'a' && ch <= 'z')
            || (ch >= 'A' && ch <= 'Z'))
        {
            lstrData += ch;
        }
        /*else if (' ' == ch)
         {
         lstrData += "+";
         } */
        else
        {
            lstrData += "%";
            lstrData += CharToHex((ch>>4)&0x0F);
            lstrData += CharToHex(ch%16);
            
        }
    }
    return lstrData;
}

string SwapCodeTools::FilterHTMLReservedCharacter(const char* p)
{
     string str = "";
     char tmp[10] = {0};
     for(size_t i = 0; i<strlen(p);i++)
     {
         memset(tmp,0,10);
           switch(p[i])
            {
            case '&':
                sprintf(tmp,"%s","&amp;");
                break;
            case '<':
                sprintf(tmp,"%s","&lt;");
                break;
            case '>':
                sprintf(tmp,"%s","&gt;");
                break;
            case '"':
                sprintf(tmp,"%s","&quot;");
                break;
            case ' ':
                sprintf(tmp,"%s","&nbsp;");
                break;
            default:
                sprintf(tmp,"%c",p[i]);
                break;

            }
            str+=tmp;
     }
     return str;
}
