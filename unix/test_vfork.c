// =======================================================================
//  FileName:     test_vfork.c
//  Author:       dingfang
//  CreateDate:   2020-12-05 16:36:23
//  ModifyAuthor: dingfang
//  ModifyDate:   2020-12-05 17:05:47
// =======================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int globvar = 6;

int main(void)
{
    int var;
    pid_t pid;

    var = 88;

    printf("before vfork\n");
    /* vfork 保证子进程先调度, 当子进程调用exec或exit后, 会开始调度父进程 */
    if ((pid = vfork()) < 0)
    {
        fprintf(stderr, "vfork error\n");
        return -1;
    }
    else if (pid == 0)
    {
        globvar++;
        var++;
        fclose(stdout);

        /* 
         * exit vfork子进程在某些系统上执行时会关闭I/O流, 
         * 导致父进程输出到I/O流时会失败 
         * 而 _exit 不会出现这种情况. 具体原因看一看exit系列部分 具体原因看一看exit系列部分
         * */
        exit(0);
        // _exit(0);
    }
    int i = printf("pid = %d, glob = %d, var = %d\n", getpid(), globvar, var);
    char buf[30] = { 0 };
    sprintf(buf, "%d\n", i);
    write(STDOUT_FILENO, buf, strlen(buf));

    return 0;
}
