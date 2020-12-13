// =======================================================================
//  FileName:     test_reentrant.c
//  Author:       dingfang
//  CreateDate:   2020-12-12 12:11:46
//  ModifyAuthor: dingfang
//  ModifyDate:   2020-12-12 12:47:57
// =======================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <signal.h>
#include <errno.h>

/*
 * getpwnam 非可重入函数, 在异步时, 
 * 可能会导致某些数据结构破坏, 从而使程序运行出现
 * 不可预期的问题
 * 该程序在运行过程中会出现不可预期的问题, 可能运行若干秒后
 * 会出现崩溃或阻塞
 * */

static void myAlarm(int signo)
{
    struct passwd *rootptr;

    printf("in signal handler\n");
    if ((rootptr = getpwnam("root")) == NULL)
    {
        fprintf(stderr, "getpwnam root error!\n");
    }
    alarm(1);
}


int main(void)
{
    struct passwd *ptr;

    signal(SIGALRM, myAlarm);
    alarm(1);

    for ( ; ; )
    {
        if ((ptr = getpwnam("dingfang")) == NULL)
        {
            fprintf(stderr, "getpwnam usr error! %s\n", strerror(errno));
        }
        if (strcmp(ptr->pw_name, "usr") != 0)
        {
            printf("return value corrupted, pwd_name = %s\n", ptr->pw_name);
        }
    }

    return 0;
}
