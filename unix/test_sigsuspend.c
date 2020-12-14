// =======================================================================
//  FileName:     test_sigsuspend.c
//  Author:       dingfang
//  CreateDate:   2020-12-14 19:57:19
//  ModifyAuthor: dingfang
//  ModifyDate:   2020-12-14 21:04:29
// =======================================================================

/*
 * 本程序对开始对SIGINT进行屏蔽
 * SIGINT在sigsuspend挂起期间，接收到SIGINT信号则将继续往下执行，并且继续被屏蔽
 * 直到屏蔽被解除，否则将继续屏蔽
 * */

#include "test_common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

static void sigInt(int signo)
{
    pr_mask("\nin sigInt: ");
}


int main(void)
{
    sigset_t newmask, oldmask, waitmask;

    pr_mask("program start: ");

    if (signal(SIGINT, sigInt) == SIG_ERR)
    {
        fprintf(stderr, "signal SIGINT failed!\n");
        return -1;
    }

    sigemptyset(&waitmask);
    sigaddset(&waitmask, SIGUSR1);
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGINT);

    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
    {
        fprintf(stderr, "sigprocmask failed!\n");
        return -1;
    }

    pr_mask("in critical region: ");

    if (sigsuspend(&waitmask) != -1)
    {
        fprintf(stderr, "sigsuspend failed!\n");
        return -1;
    }

    pr_mask("after return from sigsuspend: ");

    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
    {
        fprintf(stderr, "sigprocmask oldmask failed!\n");
        return -1;
    }

    sleep()10;

    pr_mask("program exit: ");

    return 0;
}
