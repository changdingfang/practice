// =======================================================================
//  FileName:     test_send_fd.h
//  Author:       dingfang
//  CreateDate:   2021-01-16 16:24:40
//  ModifyAuthor: dingfang
//  ModifyDate:   2021-01-16 17:19:43
// =======================================================================

#ifndef __TEST_SEND_FD_H__
#define __TEST_SEND_FD_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>

#define CONTROLLEN CMSG_LEN(sizeof(int))
#define MAXLINE 128

static struct cmsghdr *cmptr_send = NULL;
static struct cmsghdr *cmptr_recv = NULL;

int send_fd(int fd, int fd_to_send)
{
    struct iovec    iov[1];
    struct msghdr   msg;
    char            buf[2];

    iov[0].iov_base     = buf;
    iov[0].iov_len      = 2;
    msg.msg_iov         = iov;
    msg.msg_iovlen      = 1;
    msg.msg_name        = NULL;
    msg.msg_namelen     = 0;

    if (fd_to_send < 0)
    {
        msg.msg_control = NULL;
        msg.msg_controllen = 0;
        buf[1] = -fd_to_send;
        if (buf[1] == 0)
        {
            buf[1] = 1;
        }
    }
    else
    {
        if (cmptr_send == NULL && (cmptr_send = (struct cmsghdr *)malloc(CONTROLLEN)) == NULL)
        {
            return -1;
        }

        cmptr_send->cmsg_level   = SOL_SOCKET;
        cmptr_send->cmsg_type    = SCM_RIGHTS;
        cmptr_send->cmsg_len     = CONTROLLEN;
        msg.msg_control     = cmptr_send;
        msg.msg_controllen   = CONTROLLEN;

        *(int *)CMSG_DATA(cmptr_send) = fd_to_send;
        buf[1] = 0;
    }

    buf[0] = 0;
    if (sendmsg(fd, &msg, 0) != 2)
    {
        return -1;
    }
    return 0;
}


int recv_fd(int fd, ssize_t (*userfunc)(int, const void *, size_t))
{
    int newfd, nr, status;
    char buf[MAXLINE];
    struct iovec iov[1];
    struct msghdr msg;

    status = -1;
    for (;;)
    {
        iov[0].iov_base     = buf;
        iov[0].iov_len      = sizeof(buf);
        msg.msg_iov         = iov;
        msg.msg_iovlen      = 1;
        msg.msg_name        = NULL;
        msg.msg_namelen     = 0;

        if (cmptr_recv == NULL && (cmptr_recv = (struct cmsghdr *)malloc(CONTROLLEN)) == NULL)
        {
            return -1;
        }

        msg.msg_control = cmptr_recv;
        msg.msg_controllen = CONTROLLEN;

        if ((nr = recvmsg(fd, &msg, 0)) < 0)
        {
            printf("recv msg error!\n");
            return -1;
        }
        else if (nr == 0)
        {
            printf("connection closed by server\n");
            return -1;
        }

        for (char *ptr = buf; ptr < &buf[nr];)
        {
            if (*ptr++ == 0)
            {
                if (ptr != &buf[nr - 1])
                {
                    printf("message format error\n");
                    exit(-1);
                }
                status = *ptr & 0xFF;
                if (status == 0)
                {
                    if (msg.msg_controllen < CONTROLLEN)
                    {
                        printf("status = 0 but no fd\n");
                        exit(-1);
                    }
                    newfd = *(int *)CMSG_DATA(cmptr_recv);
                }
                else
                {
                    newfd = -status;
                }
                nr -= 2;
            }
        }

        if (nr > 0 && (*userfunc)(STDERR_FILENO, buf, nr) != nr)
        {
            return -1;
        }
        if (status >= 0)
        {
            return newfd;
        }
    }
}


#endif /* __TEST_SEND_FD_H__ */
