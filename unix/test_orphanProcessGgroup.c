// =======================================================================
//  FileName:     test_orphanProcessGgroup.c
//  Author:       dingfang
//  CreateDate:   2020-12-12 09:53:21
//  ModifyAuthor: dingfang
//  ModifyDate:   2020-12-12 10:05:30
// =======================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>

static void sigHup(int signo)
{
    printf("SIGHUP received, pid = %ld\n", (long)getpid());
}


static void prIds(const char *name)
{
    printf("%s: pid = %ld, ppid = %ld, pgrp = %ld, tpgrp = %ld\n",
            name, (long)getpid(), (long)getppid(), (long)getpgrp(),
            (long)tcgetpgrp(STDIN_FILENO));
    fflush(stdout);
}


int main(void)
{
    pid_t pid;

    prIds("parent");

    if ((pid = fork()) < 0)
    {
        fprintf(stderr, "fork error! %s", strerror(errno));
        exit(-1);
    }
    else if (pid > 0)
    {
        sleep(5);
    }
    else
    {
        prIds("child");
        signal(SIGHUP, sigHup);
        kill(getpid(), SIGTSTP);
        prIds("child");
        char c;
        if (read(STDIN_FILENO, &c, 1) != 1)
        {
            printf("read error %d on controlling TTY\n", errno);
        }
    }

    exit(0);

    return 0;
}
