// =======================================================================
//  FileName:     test.c
//  Author:       dingfang
//  CreateDate:   2021-01-16 17:26:49
//  ModifyAuthor: dingfang
//  ModifyDate:   2021-01-16 20:21:19
// =======================================================================

#include "open.h"
#include "test_send_fd.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <stddef.h>
#include <sys/un.h>
#include <errno.h>

#define MAXARGC 50
#define WHILE " \t\n"

int oflag;
char *pathname;

int buf_args(char *buf, int (*optfunc)(int, char **))
{
    char *ptr, *argv[MAXARGC];
    int argc;

    if (strtok(buf, WHILE) == NULL)
    {
        return -1;
    }

    argv[argc = 0] = buf;

    while ((ptr = strtok(NULL, WHILE)) != NULL)
    {
        if (++argc >= MAXARGC - 1)
        {
            return -1;
        }
        argv[argc] = ptr;
    }
    argv[++argc] = NULL;

    return ((*optfunc)(argc, argv));
}


int cli_args(int argc, char **argv)
{
    if (argc != 3 || strcmp(argv[0], CL_OPEN) != 0)
    {
        return -1;
    }
    pathname = argv[1];
    oflag = atoi(argv[2]);
    return 0;
}


void handle_request(char *buf, int nread, int fd)
{
    int newfd;

    if (buf[nread - 1] != 0)
    {
        printf("handle_request error\n");
        return ;
    }

    if (buf_args(buf, cli_args) < 0)
    {
        printf("buf_args failed\n");
        return ;
    }

    if ((newfd = open(pathname, oflag)) < 0)
    {
        printf("open failed\n");
        return ;
    }

    if (send_fd(fd, newfd) < 0)
    {
        printf("send_fd error!\n");
        printf("error info: %s\n", strerror(errno));
    }

    close(newfd);
}


int main(void)
{
    int nread;
    char buf[MAXLINE];

    for (;;)
    {
        if ((nread = read(STDIN_FILENO, buf, MAXLINE)) < 0)
        {
            printf("read failed\n");
        }
        else if (nread == 0)
        {
            break;
        }

        handle_request(buf, nread, STDOUT_FILENO);
        break;
    }
    

    return 0;
}
