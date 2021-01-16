// =======================================================================
//  FileName:     test_cli.c
//  Author:       dingfang
//  CreateDate:   2021-01-16 18:14:12
//  ModifyAuthor: dingfang
//  ModifyDate:   2021-01-16 20:36:47
// =======================================================================

#include "test_send_fd.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <sys/un.h>
#include <errno.h>

#define BUFFSIZE 8192

int csopen()
{
    /* ====================================== */
    int fd;
    struct sockaddr_un un;
    const char *unix_path =  "./unix_sock.sock";
    un.sun_family = AF_UNIX;
    strcpy(un.sun_path, unix_path);
    int size = offsetof(struct sockaddr_un, sun_path) + strlen(un.sun_path);

    fd = socket(AF_UNIX, SOCK_STREAM, 0);
    printf("wile connect server\n");
    if (connect(fd, (struct sockaddr *)&un, size) < 0)
    {
        printf("conect failed\n");
        printf("error info : %s\n", strerror(errno));
        exit(-1);
    }
    printf("wile connect successful\n");
    /* ====================================== */

    return recv_fd(fd, write);
}

int main(void)
{
    int n, fd;
    char buf[BUFFSIZE];

    if ((fd = csopen()) < 0)
    {
        printf("init fd error\n");
        return -1;
    }

    while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0)
    {
        if (write(fd, buf, n) != n)
        {
            return -1;
        }
    }

    close(fd);

    return 0;
}
