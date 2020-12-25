// =======================================================================
//  FileName:     test_syslog.c
//  Author:       dingfang
//  CreateDate:   2020-12-24 08:38:56
//  ModifyAuthor: dingfang
//  ModifyDate:   2020-12-24 09:07:50
// =======================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>


int main(void)
{
    openlog("test_syslog", LOG_CONS, LOG_DAEMON);

    /* 写入到 /var/log/messages */
    syslog(LOG_INFO, "hello world haha");
    closelog();
            

    return 0;
}
