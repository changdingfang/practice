// =======================================================================
//  FileName:     test_unix_sock.c
//  Author:       dingfang
//  CreateDate:   2021-01-16 10:33:27
//  ModifyAuthor: dingfang
//  ModifyDate:   2021-01-16 10:45:19
// =======================================================================

#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/un.h>
#include <sys/socket.h>

int main(void)
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

    if (access(unix_path, F_OK) == 0)
    {
        remove(unix_path);
    }

    if (bind(fd, (struct sockaddr *)&un, size) < 0)
    {
        printf("bind failed!\n");
        exit(-1);
    }

    printf("UNIX domain socket bound\n");

    return 0;
}
