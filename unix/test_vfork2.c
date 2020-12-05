// =======================================================================
//  FileName:     test_vfork2.c
//  Author:       dingfang
//  CreateDate:   2020-12-05 17:05:55
//  ModifyAuthor: dingfang
//  ModifyDate:   2020-12-05 17:19:53
// =======================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
/*
 * 错误使用vfork例子
 * 父子进程共用函数堆栈, vfork让子进程优先执行,并退出testVfork函数栈,
 * 在后面父进程该testVfork已经被退出, 再继续执行下去, 会发生不可预期的错误,
 * 很可能出现core
 * */
void testVfork()
{
    pid_t pid;

    printf("before vfork\n");
    /* vfork 保证子进程先调度, 当子进程调用exec或exit后, 会开始调度父进程 */
    if ((pid = vfork()) < 0)
    {
        fprintf(stderr, "vfork error\n");
        return ;
    }
    else if (pid == 0)
    {
        printf("child pid: (%d)\n", getpid());
        return ;
    }
    else
    {
        sleep(3);
        printf("parent pid: (%d)\n", getpid());
    }


    return ;
}

int main(void)
{
    testVfork();

    printf("main: pid: (%d), ppid(%d)\n", getpid(), getppid());
    return 0;
}
