// =======================================================================
//  FileName:     test_sleep.c
//  Author:       dingfang
//  CreateDate:   2020-12-13 10:31:01
//  ModifyAuthor: dingfang
//  ModifyDate:   2020-12-13 10:47:14
// =======================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <setjmp.h>

static jmp_buf envAlrm;

static void sigAlrm(int signo)
{
    longjmp(envAlrm, 1);
}


static void sigInt(int signo)
{
    printf("sigInt starting\n");

    volatile int k;

    /* 使该运行时间大于sleep1的睡眠时间, 让INT信号处理陷于此处 */
    for (int i = 0; i < 1000 * 1000 * 1000; ++i)
    {
        for (int j = 0; j < 1000 * 1000; ++j)
        {
            k = i * j;
        }
    }

    printf("sigInt finished\n");
}

unsigned int sleep1(unsigned int sec)
{
    if (signal(SIGALRM, sigAlrm) == SIG_ERR)
    {
        return (sec);
    }
    if (setjmp(envAlrm) == 0)
    {
        alarm(sec);
        pause();
    }

    return (alarm(0));
}


int main(void)
{
    unsigned int unslept;

    if (signal(SIGINT, sigInt) == SIG_ERR)
    {
        fprintf(stderr, "signal failed!\n");
        return -1;
    }

    /* 
     * 真sleep在接收到其它信号不会中断信号的处理, 
     * 而是等信号处理结束后再继续运行 
     * 而sleep1却会中断其它信号处理, 一旦sleep1执行
     * 完毕,就会继续往下执行
     * */
    unslept = sleep1(5);
    // unslept = sleep(5);
    printf("sleep1 returned: %d\n", unslept);

    return 0;
}
