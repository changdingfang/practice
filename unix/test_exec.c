// =======================================================================
//  FileName:     test_exec.c
//  Author:       dingfang
//  CreateDate:   2020-12-05 12:09:42
//  ModifyAuthor: dingfang
//  ModifyDate:   2020-12-05 13:32:23
// =======================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

int main(void)
{
    pid_t pid;

    const char *envInit[] = { "USER=unknow", "PATH=/tmp", NULL };
    if ((pid = fork()) < 0)
    {
        fprintf(stderr, "fork error\n");
        return -1;
    }
    else if (pid == 0)
    {
        printf("execle before pid: [%d]\n", getpid());
        if (execle("./echoall", 
                    "echoall",
                    "myargl",
                    "MY_ARG2",
                    (char *)0,
                    envInit) < 0)
        {
            fprintf(stderr, "fork error\n");
            return -1;
        }
        printf("pid: [%d]\n", getpid());
    }

    if (waitpid(pid, NULL, 0) < 0)
    {
        fprintf(stderr, "fork error\n");
        return -1;
    }

    if ((pid = fork()) < 0)
    {
        fprintf(stderr, "fork error\n");
        fprintf(stderr, "errinfo: %s\n", strerror(errno));
        return -1;
    }
    else if (pid == 0)
    {
        if (execlp("./echoall", "echoall", "only 1 arg", (char *)0) < 0)
        {
            fprintf(stderr, "execlp error\n");
            fprintf(stderr, "errinfo: %s\n", strerror(errno));
            return -1;
        }
    }

    return 0;
}
