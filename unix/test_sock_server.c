// =======================================================================
//  FileName:     test_sock_server.c
//  Author:       dingfang
//  CreateDate:   2021-01-13 21:08:11
//  ModifyAuthor: dingfang
//  ModifyDate:   2021-01-13 21:34:05
// =======================================================================

/*
 *  多余的程序，需要重新编写
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/wait.h>


int set_cloexec(int fd)
{
    int val;

    if ((val = fcntl(fd, F_GETFD, 0)) < 0)
    {
        return -1;
    }

    val |= FD_CLOEXEC;
    return (fcntl(fd, F_SETFD, val));
}

void serve(int sockfd)
{
    int clfd, status;
    pid_t pid;

    set_cloexec(sockfd);

    for (;;)
    {
        if ((clfd = accept(sockfd, NULL, NULL)) <0)
        {
            printf("ruptimed: accept error: %s\n", strerror(errno));
            exit(-1);
        }

        if ((pid = fork()) < 0)
        {
            printf("fork failed!\n");
            exit (-1);
        }
        else if (pid == 0)
        {
            if (dup2(clfd, STDOUT_FILENO) != STDOUT_FILENO ||
                    dup2(clfd, STDERR_FILENO) != STDERR_FILENO)
            {
                printf("dup2 failed!\n");
                exit(-1);
            }

            close(clfd);
            execl("/usr/bin/uptime", "uptime", (char *)0);
        }
        else
        {
            close(clfd);
            waitpid(pid, &status, 0);
        }
    }
}

int main(int argc, char **argv)
{
    struct addrinfo *alist, *api;
    int sockfd, err, n;
    char *host;

    if (argc != 1)
    {
        printf("argc != 1\n");
        exit(-1);
    }
    if ((n = sysconf(_SC_HOST_NAME_MAX)) < 0)
    {
        n = 256;
    }

    if ((host = malloc(n)) == NULL)
    {
        printf("malloc error!\n");
        exit(-1);
    }

    if (gethostname(host, n) < 0)
    {
        printf("gethostname error!\n");
        exit(-1);
    }

#if 0
    struct addinfo hint;
    memset(&hint, 0, sizeof(hint));
    hint.ai_flags = AI_CANONNAME;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_canonname = NULL;
    hint.ai_addr = NULL;
    hint.ai_next = NULL;
#endif


    return 0;
}
