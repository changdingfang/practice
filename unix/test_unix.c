// =======================================================================
//  FileName:     test_unix.c
//  Author:       dingfang
//  CreateDate:   2021-01-16 09:51:28
//  ModifyAuthor: dingfang
//  ModifyDate:   2021-01-16 10:27:43
// =======================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <poll.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/msg.h>

#define NQ      3
#define MAXMSZ  512
#define KEY     0x123

struct threadinfo
{
    int qid;
    int fd;
};

struct mymesg
{
    long mtype;
    char mtext[MAXMSZ];
};

void *helper(void *arg)
{
    int n;
    struct mymesg m;
    struct threadinfo *tip = arg;

    for (;;)
    {
        memset(&m, 0x00, sizeof(m));
        if ((n = msgrcv(tip->qid, &m, MAXMSZ, 0, MSG_NOERROR)) < 0)
        {
            printf("msgrcv failed\n");
            exit(-1);
        }

        if (write(tip->fd, m.mtext, n) < 0)
        {
            printf("write failed\n");
            exit(-1);
        }
        printf("hello world pid: %ld\n", pthread_self());
    }
}

int main(void)
{
    int             n;
    int             fd[2];
    int             qid[NQ];
    struct pollfd   pfd[NQ];
    pthread_t       tid[NQ];
    char            buf[MAXMSZ];
    struct threadinfo ti[NQ];

    for (int i = 0; i < NQ; ++i)
    {
        if ((qid[i] = msgget((KEY + i), IPC_CREAT | 0666)) < 0)
        {
            printf("msgget failed!\n");
            exit(-1);
        }

        if (socketpair(AF_UNIX, SOCK_DGRAM, 0, fd) < 0)
        {
            printf("socketpair failed!\n");
            exit(-1);
        }

        pfd[i].fd       = fd[0];
        pfd[i].events   = POLLIN;
        ti[i].qid       = qid[i];
        ti[i].fd        = fd[1];
        if (pthread_create(&tid[i], NULL, helper, &ti[i]) != 0)
        {
            printf("pthread_create failed!\n");
            exit(-1);
        }
    }

    for (;;)
    {
        if (poll(pfd, NQ, -1) < 0)
        {
            printf("poll failed!\n");
            exit(-1);
        }

        for (int i = 0; i < NQ; ++i)
        {
            memset(buf, 0x00, sizeof(buf));
            if (pfd[i].revents & POLLIN)
            {
                if ((n = read(pfd[i].fd, buf, sizeof(buf))) < 0)
                {
                    printf("read failed!\n");
                    exit(-1);
                }
                buf[n] = '\0';
                printf("queue id %d, msg: %s\n", qid[i], buf);
            }
        }
    }

    return 0;
}
