// =======================================================================
//  FileName:     test_sigwait.c
//  Author:       dingfang
//  CreateDate:   2020-12-23 19:31:32
//  ModifyAuthor: dingfang
//  ModifyDate:   2020-12-23 19:43:50
// =======================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>

int quitflag;
sigset_t mask;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t waitloc = PTHREAD_COND_INITIALIZER;

void *thrFn(void *arg)
{
    int err, signo;

    for ( ; ; )
    {
        err = sigwait(&mask, &signo);
        if (err != 0)
        {
            printf("sigwait error!\n");
            exit(-1);
        }

        switch (signo)
        {
            case SIGINT:
                printf("\nSIGINT\n");
                break;
            case SIGQUIT:
                pthread_mutex_lock(&lock);
                printf("\nSIGQUIT\n");
                quitflag = 1;
                pthread_mutex_unlock(&lock);
                pthread_cond_signal(&waitloc);
                return NULL;
            default:
                printf("unexpected signal %d\n", signo);
                exit(-1);
        }
    }
}

int main(void)
{
    pthread_t tid;

    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGQUIT);

    if (pthread_sigmask(SIG_BLOCK, &mask, NULL) != 0)
    {
        printf("pthread sigmask error!\n");
        exit(-1);
    }

    if (pthread_create(&tid, NULL, thrFn, NULL) != 0)
    {
        printf("pthread create error!\n");
        exit(-1);
    }

    /* 防止信号发生时未来得及置位flag导致一直阻塞 */
    pthread_mutex_lock(&lock);
    while (quitflag == 0)
    {
        pthread_cond_wait(&waitloc, &lock);
    }

    pthread_mutex_unlock(&lock);

    quitflag = 0;

    return 0;
}
