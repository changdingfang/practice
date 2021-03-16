// =======================================================================
//  FileName:     test_tcp_get_time_server_v2.c
//  Author:       dingfang
//  CreateDate:   2021-03-16 19:36:05
//  ModifyAuthor: dingfang
//  ModifyDate:   2021-03-16 20:31:39
// =======================================================================

#include "unp.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    int listenfd;
    socklen_t addrlen;

    if (argc == 2)
    {
        listenfd = tcpListen(NULL, argv[1], &addrlen);
    }
    else if (argc == 3)
    {
        listenfd = tcpListen(argv[1], argv[2], &addrlen);
    }
    else
    {
        err_quit("usage: daytime tcpsrv2 [ <host> ] <service or prot>");
    }


    struct sockaddr_storage cliaddr;
    struct sockaddr_in sa;
    int connfd;
    time_t ticks;
    socklen_t len;
    char buff[MAXLINE];
    while (1)
    {
        len = sizeof(cliaddr);
        connfd = Accept(listenfd, (SA *)&cliaddr, &len);
        printf("connection from %s\n", inet_ntop(AF_INET, (const void *)&cliaddr, (char *)&sa, sizeof(SA)));

        ticks = time(NULL);
        snprintf(buff ,sizeof(buff), "%.24s\r\n", ctime(&ticks));
        Write(connfd, buff, strlen(buff));

        Close(connfd);
    }

    return 0;
}
