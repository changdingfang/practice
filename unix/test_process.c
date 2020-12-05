// =======================================================================
//  FileName:     test_process.c
//  Author:       dingfang
//  CreateDate:   2020-12-04 08:48:06
//  ModifyAuthor: dingfang
//  ModifyDate:   2020-12-04 22:50:02
// =======================================================================

/*
 * 父进程可能很久不会退出，而又不想wait不知何时会退出子进程，
 * 这时fork两次可以做到让子进程里面退出，子进程的子进程成为孤儿进程。
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    pid_t pid;

    if ((pid = fork()) < 0)
    {
        fprintf(stderr, "fork error!\n");
        return -1;
    }
    else if (pid == 0)
    {
        fprintf(stdout, "c: pid: %d\n", getpid());
        fprintf(stdout, "c: ppid: %d\n", getppid());
        if (fork() == 0)
        {
            fprintf(stdout, "s: pid: %d\n", getpid());
            fprintf(stdout, "s: ppid: %d\n", getppid());
            sleep(10);
            fprintf(stdout, "s: pid: %d\n", getpid());
            fprintf(stdout, "s: ppid: %d\n", getppid());
            exit(-1);
        }

        exit(-1);
    }

    fprintf(stdout, "pid: %d\n", getpid());
    sleep(5);


    return 0;
}
