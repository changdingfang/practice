// =======================================================================
//  FileName:     test_tcp_get_time_server.c
//  Author:       dingfang
//  CreateDate:   2021-01-27 21:59:26
//  ModifyAuthor: dingfang
//  ModifyDate:   2021-01-27 22:12:18
// =======================================================================

#include "unp.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    int listenfd, connfd;
    struct sockaddr_in servaddr;
    char buff[MAXLINE];
    time_t ticks;

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    memset(&servaddr, 0x00, sizeof(servaddr));
    servaddr.sin_family         = AF_INET;
    servaddr.sin_addr.s_addr    = htonl(INADDR_ANY);
    servaddr.sin_port           = htons(4455);
    
    Bind(listenfd, (SA *)&servaddr, sizeof(servaddr));

    Listen(listenfd, LISTENQ);

    for ( ;; )
    {
        connfd = Accept(listenfd, NULL, NULL);

        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        Write(connfd, buff, strlen(buff));
        Close(connfd);
    }


    return 0;
}
