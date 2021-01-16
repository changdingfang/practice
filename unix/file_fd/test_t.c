// =======================================================================
//  FileName:     test.c
//  Author:       dingfang
//  CreateDate:   2021-01-16 17:26:49
//  ModifyAuthor: dingfang
//  ModifyDate:   2021-01-16 20:38:17
// =======================================================================

#include "test_send_fd.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <stddef.h>
#include <sys/un.h>
#include <errno.h>

int unix_sock()
{
    int fd, size;
    struct sockaddr_un un;

    const char *unix_path =  "./unix_sock.sock";
    un.sun_family = AF_UNIX;
    strcpy(un.sun_path, unix_path);

    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
    {
        printf("socket failed!\n");
        exit(-1);
    }

    size = offsetof(struct sockaddr_un, sun_path) + strlen(un.sun_path);

    struct stat statbuf;
    if (stat(unix_path, &statbuf) == 0)
    {
        if (S_ISSOCK(statbuf.st_mode) == 0)
        {
            printf("file alread exists and not sock type, unxi path: %s\n", unix_path);
            exit(-1);
        }
        remove(unix_path);
    }

    if (bind(fd, (struct sockaddr *)&un, size) < 0)
    {
        printf("bind failed!\n");
        exit(-1);
    }

    listen(fd, 10);

    return fd;
}


void handle_request(int fd)
{
    if (send_fd(fd, STDOUT_FILENO) < 0)
    {
        printf("send_fd error!\n");
        printf("error info: %s\n", strerror(errno));
    }

    close(STDOUT_FILENO);
}


int main(void)
{
    int fd = unix_sock();

    int clifd = accept(fd, NULL, NULL);
    printf("cli connect ...\n");
    handle_request(clifd);

    return 0;
}
