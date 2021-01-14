// =======================================================================
//  FileName:     test_sock.h
//  Author:       dingfang
//  CreateDate:   2021-01-14 21:35:17
//  ModifyAuthor: dingfang
//  ModifyDate:   2021-01-14 21:39:31
// =======================================================================

#ifndef __TEST_SOCK_H__
#define __TEST_SOCK_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>

int initserver(int type, const struct sockaddr *addr, socklen_t alen, int qlen)
{
    int fd, err;
    int reuse = 1;

    if ((fd = socket(addr->sa_family, type, 0)) < 0)
    {
        fprintf(stderr, "socket init error!!\n");
        return -1;
    }

    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) < 0)
    {
        goto errout;
    }

    if (bind(fd, addr, alen) < 0)
    {
        goto errout;
    }

    if (type == SOCK_STREAM || type == SOCK_SEQPACKET)
    {
        if (listen(fd, qlen) < 0)
        {
            goto errout;
        }
    }

errout:
    err = errno;
    close(fd);
    errno = err;
    return -1;

    return fd;
}



#endif /* __TEST_SOCK_H__ */
