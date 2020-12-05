// =======================================================================
//  FileName:     test_daemon.c
//  Author:       dingfang
//  CreateDate:   2020-12-04 19:48:02
//  ModifyAuthor: dingfang
//  ModifyDate:   2020-12-04 22:49:53
// =======================================================================

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

int myDaemon()
{
    int fd;
    switch (fork())
    {
        case -1:
            fprintf(stderr, "fork failed!\n");
            return -1;
        case 0:
            break;
        default:
            exit(0);
    }

    pid_t pid = getpid();

    if (setsid() == -1)
    {
        fprintf(stderr, "setsid failed!\n");
        return -1;
    }
    umask(0);
    fd = open("/dev/null", O_RDWR);
    if (fd == -1)
    {
        fprintf(stderr, "open /dev/null failed!\n");
        return -1;
    }

    if (dup2(fd, STDIN_FILENO) == -1)
    {
        fprintf(stderr, "dup2 STDIN failed!\n");
        return -1;
    }

    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        fprintf(stderr, "dup2 STDOUT failed!\n");
        return -1;
    }

    if (dup2(fd, STDERR_FILENO) == -1)
    {
        fprintf(stderr, "dup2 STDERR failed!\n");
        return -1;
    }

    if (fd > STDERR_FILENO)
    {
        if (close(fd) == -1)
        {
            fprintf(stderr, "close failed!\n");
            return -1;
        }
    }

    sleep(100);

    return 0;
}

int main(void)
{
    myDaemon();

    return 0;
}
