// =======================================================================
//  FileName:     test_getaddrinfo.c
//  Author:       dingfang
//  CreateDate:   2021-03-15 20:08:49
//  ModifyAuthor: dingfang
//  ModifyDate:   2021-03-16 18:23:26
// =======================================================================

#include "unp.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    struct addrinfo hints, *ptr = NULL, *res = NULL;

    memset(&hints, 0x00, sizeof(struct addrinfo));

    hints.ai_flags  = AI_CANONNAME;
    hints.ai_family = AF_INET;

    if (getaddrinfo("izuf63269bclc8z39n3t2cz", "daytime", &hints, &res) < 0)
    {
        err_quit("getaddrinfo error!");
        /* return -1; */
    }

    ptr = res;
    while (ptr)
    {
        printf("canonname: %s\n", ptr->ai_canonname);
        switch (ptr->ai_family)
        {
            case AF_INET: printf("family is AF_INET\n"); break;
            case AF_INET6: printf("family is AF_INET6\n"); break;
            default: printf("not found family\n");
        }

        switch (ptr->ai_socktype)
        {
            case SOCK_STREAM: printf("socktype is SOCK_STREAM\n"); break;
            case SOCK_DGRAM: printf("socktype is SOCK_DGRAM\n"); break;
            default: printf("not found sock type\n");
        }

        switch (ptr->ai_protocol)
        {
            case IPPROTO_UDP: printf("sprotocol is IPPROTO_UDP\n"); break;
            case IPPROTO_TCP: printf("sprotocol is IPPROTO_TCP\n"); break;
            default: printf("not found protocol type\n");
        }
        printf("---------------------\n");
        ptr = ptr->ai_next;
    }

    freeaddrinfo(res);

    return 0;
}

