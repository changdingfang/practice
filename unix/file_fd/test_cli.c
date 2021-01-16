// =======================================================================
//  FileName:     test_cli.c
//  Author:       dingfang
//  CreateDate:   2021-01-16 18:14:12
//  ModifyAuthor: dingfang
//  ModifyDate:   2021-01-16 20:21:53
// =======================================================================

#include "open.h"
#include "test_send_fd.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stddef.h>
#include <sys/un.h>
#include <errno.h>

#define BUFFSIZE 8192

int fd_pipe(int fd[2])
{
    return socketpair(AF_UNIX, SOCK_STREAM, 0, fd);
}


int csopen(char *name, int oflag)
{
    pid_t       pid;
    int         len;
    char        buf[10];
    struct iovec    iov[3];
    static int      fd[2] = { -1, -1 };

    if (fd[0] < 0)
    {
        if (fd_pipe(fd) < 0)
        {
            return -1;
        }

        if ((pid = fork()) < 0)
        {
            printf("fork error\n");
            return -1;
        }
        else if (pid == 0)
        {
            close(fd[0]);
            if (fd[1] != STDIN_FILENO && dup2(fd[1], STDIN_FILENO) != STDIN_FILENO)
            {
                printf("dup2 stdin error!\n");
            }

            if (fd[1] != STDOUT_FILENO && dup2(fd[1], STDOUT_FILENO) != STDOUT_FILENO)
            {
                printf("dup2 stdout error!\n");
            }

            if (execl("./opend", "opend", (char *)0) < 0)
            {
                printf("execl failed!\n");
            }
        }

        close(fd[1]);
    }

    sprintf(buf, " %d", oflag);
    iov[0].iov_base     = CL_OPEN " ";;
    iov[0].iov_len      = strlen(CL_OPEN) + 1;
    /* 文件名 */
    iov[1].iov_base     = name;
    iov[1].iov_len      = strlen(name);
    /* 打开文件的权限 */
    iov[2].iov_base     = buf;
    iov[2].iov_len      = strlen(buf) + 1; /* +1 补充上一个 '\0' */
    len = iov[0].iov_len + iov[1].iov_len + iov[2].iov_len;

    if (writev(fd[0], &iov[0], 3) != len)
    {
        printf("writev error!\n");
        return -1;
    }

    return recv_fd(fd[0], write);
}

int main(void)
{
    int n, fd;
    char buf[BUFFSIZE];
    char line[MAXLINE];

    while (fgets(line, MAXLINE, stdin) != NULL)
    {
        if (line[strlen(line) - 1] == '\n')
        {
            line[strlen(line) - 1] = 0;
        }
        printf("line: %s\n", line);

        if ((fd = csopen(line, O_RDONLY)) < 0)
        {
            continue;
        }

        while ((n = read(fd, buf, BUFFSIZE)) > 0)
        {
            if (write(STDOUT_FILENO, buf, n) != n)
            {
                return -1;
            }
        }

        if (n < 0)
        {
            printf("read error");
            return -1;
        }

        close(fd);
    }

    return 0;
}
