// =======================================================================
//  FileName:     test_get_mss_snd.c
//  Author:       dingfang
//  CreateDate:   2021-02-22 20:50:33
//  ModifyAuthor: dingfang
//  ModifyDate:   2021-02-22 21:13:25
// =======================================================================

#include "unp.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
 * 输出connect前后，recvbuf和mss的大小。
 * */

int main(int argc, char **argv)
{
    int sockfd, recvbuf, mss;
    socklen_t len;
    if (argc != 2)
    {
        err_quit("usage: recvbuf <IPaddress>");
    }

    sockfd = Socket(AF_INET, SOCK_STREAM, 0);

    len = sizeof(recvbuf);
    Getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &recvbuf, &len);
    len = sizeof(mss);
    Getsockopt(sockfd, IPPROTO_TCP, TCP_MAXSEG, &mss, &len);

    printf("conn befoer: \n");
    printf("recvbuf\t= %d\nMSS\t= %d\n", recvbuf, mss);

    struct sockaddr_in servaddr;
    memset(&servaddr, 0x00, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(4455);
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) < 0)
    {
        err_quit("connect error!");
    }

    len = sizeof(recvbuf);
    Getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &recvbuf, &len);
    len = sizeof(mss);
    Getsockopt(sockfd, IPPROTO_TCP, TCP_MAXSEG, &mss, &len);

    printf("conn after: \n");
    printf("recvbuf\t= %d\nMSS\t= %d\n", recvbuf, mss);

    return 0;
}
