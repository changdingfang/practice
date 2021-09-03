#include "base64.h"

#include <string.h>
#include <time.h>

#include <sys/socket.h>
#include <netdb.h>
//#include <fcntl.h>
//#include <arpa/inet.h>
//#include <unistd.h>
#include <string>
#include <iostream>
//#include <fstream>

using namespace std;

const string		fromMailPassWd_	= "fd245292011";
const string		fromMail_		= "15738938828@163.com";
const string		smtpServer_		= "smtps://smtp.163.com";
//const string		smtpServer_		= "smtp.aliyun.com";
const int smtpPort_		= 465;
int fd_ = 0;

int connectMail()
{
	fd_ = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == fd_)
	{
		cout << "socket fd error" << endl;
		return -1;
	}
	struct hostent *remoteHost;
	if ((remoteHost = gethostbyname(smtpServer_.c_str())) == 0)
	{
		cout << "get host failed host: " << smtpServer_ << endl;
		return -1;
	}
	struct sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = ((struct in_addr *)(remoteHost->h_addr))->s_addr;
	sockAddr.sin_port = htons(smtpPort_);
	//cout << __LINE__ << "   remote host ip: " << ((struct in_addr *)(remoteHost->h_addr))->s_addr << endl;
	//cout << __LINE__ << "   remote host ip: " << sockAddr.sin_addr.s_addr << endl;

	if (connect(fd_, (const sockaddr*)&sockAddr, sizeof(sockAddr)) == -1)
	{
		cout << __LINE__ << "   connect error" << endl;
		return -1;
	}

	return 0;
}

int main(void)
{
	connectMail();

	cout << "connect succ...." << endl;
	char buff[2048] = {0};
	recv(fd_, buff, 2048, 0);
	string hel = "HELO smtp\r\n";
	cout << "buf: " << buff << endl;

	send(fd_, hel.c_str(), hel.length(), 0);
	memset(buff, 0, 2048);
	recv(fd_, buff, 2047, 0);
	string login = "auth login\r\n";

	send(fd_, login.c_str(), login.length(), 0);
	memset(buff, 0, 2048);
	recv(fd_, buff, 2047, 0);
	cout << "send login: " << buff << endl;

	string login_name = "15738938828@163.com";
	string login_passwd = "fd245292011";
	login_name = (base64_encode(reinterpret_cast<const unsigned char*>(login_name.c_str()), login_name.length()));
	login_name.append("\r\n");
	send(fd_, login_name.c_str(), login_name.length(), 0);
	memset(buff, 0, 2048);
	recv(fd_, buff, 2047, 0);
	cout << "send login_name: " << buff << endl;

	login_passwd = (base64_encode(reinterpret_cast<const unsigned char*>(login_passwd.c_str()), login_passwd.length()));
	login_passwd.append("\r\n");
	send(fd_, login_passwd.c_str(), login_passwd.length(), 0);
	memset(buff, 0, 2048);
	recv(fd_, buff, 2047, 0);
	cout << "send login_passwd: " << buff << endl;

	cout << "connect success " << endl;









	string	msg;
	msg = "mail from:<";
	msg.append(fromMail_);
	msg.append(">\n");

	string	toMail = "245292011@qq.com";
	msg.append("rcpt to:<");
	msg.append(toMail);
	msg.append(">\n");

	string	subject = "test ding fang";
	string	content = "this is content";
	msg.append("data\n");
	msg.append("subject:");
	msg.append(subject);
	msg.append("\n\n");
	msg.append(content);
	msg.append("\n.\n");

	if (send(fd_, msg.c_str(), msg.length(), 0) == -1)
	{
		cout << __LINE__ << "  send error" << endl;
		return -1;
	}
	char recvBuf[2048] = {0};
	int len = recv(fd_, recvBuf, 2047, 0);
	cout << "len: " << len << endl;
	cout << "recvBuf: " << recvBuf << endl;

	return 0;
}
