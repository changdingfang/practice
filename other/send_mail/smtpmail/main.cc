/********************************************************************
 * author: waponx
 * email : 18826419224@163.com
 * time  : 10.11.2015
 * ******************************************************************/

#include "smtpmail.h"

#include <string>

int main() {
	using Mail::SMTPMail;
	SMTPMail *sm = new SMTPMail(SMTPMail::String("15738938828@163.com"),
			SMTPMail::String("qq245292011"),
			SMTPMail::String("smtp.163.com"));

	sm->SendEmail("245292011@qq.com", "test, 简单的测试, 丁方");
	delete sm;

	return 0;
}
