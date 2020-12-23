// =======================================================================
//  FileName:     test_daemon_2.c
//  Author:       dingfang
//  CreateDate:   2020-12-23 21:31:49
//  ModifyAuthor: dingfang
//  ModifyDate:   2020-12-23 21:43:54
// =======================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <sys/resource.h>

void daemonize(const char *cmd)
{
    pid_t pid;
    struct rlimit rl;
    struct sigaction sa;

    umask(0);

    if (getrlimit(RLIMIT_NOFILE, &rl) < 0)
    {
        printf("can't get file limit\n");
        exit(-1);
    }

    if ((pid = fork()) < 0)
    {
        printf("%s: can't fork\n", cmd);
        exit(-1);
    }
    else if (pid != 0)
    {
        exit(0);
    }

    setsid();

    sa.sa_handler = SIG_IGN;

    sigemptyset(&sa.sa_mask);
    sa.sa_flags  =0;
    if (sigaction(SIGHUP, &sa, NULL) < 0)
    {
        printf("%s: can't ignore SIGHUP\n", cmd);
        exit(-1);
    }
 
    if ((pid = fork()) < 0)
    {
        printf("%s: can't fork 2\n", cmd);
        exit(-1);
    }
    else if (pid != 0)
    {
        exit(0);
    }

    if (chdir("/") < 0)
    {
        printf("%s: can't change directory to /\n", cmd);
        exit(-1);
    }

    if (rl.rlim_max == RLIM_INFINITY)
    {
        rl.rlim_max = 1024;
    }

    for (int i = 0; i < rl.rlim_max; ++i)
    {
        close(i);
    }

    int fd0 = open("/dev/null", O_RDWR);
    int fd1 = dup(0); 
    int fd2 = dup(0); 
}

int main(void)
{

    daemonize("sleep 10");
    /* 该程序将成为守护进程，父进程为1号进程 */
    sleep(10);

    return 0;
}
