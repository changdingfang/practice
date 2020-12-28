// =======================================================================
//  FileName:     test_daemon_reread.c
//  Author:       dingfang
//  CreateDate:   2020-12-28 19:14:22
//  ModifyAuthor: dingfang
//  ModifyDate:   2020-12-28 19:20:59
// =======================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>


sigset_t mask;

void *thrFn(void *arg)
{
    int signo;

    while (1)
    {
        if (sigwait(&mask, &signo) !=0)
        {
            printf("sigwait failed!\n");
            exit(-1);
        }

        switch (signo)
        {
            case SIGHUP: /* reread() */ printf("SIGHUP\n"); break;
            case SIGTERM: printf("SIGTERM\n"); exit(0);
            default: printf("default\n"); break;
        }
    }

    return NULL;
}

int main(void)
{

    struct sigaction sa;

    sa.sa_handler = SIG_DFL;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGHUP, &sa, NULL) < 0)
    {
        printf("sigaction  !\n");
        exit(-1);
    }

    sigfillset(&mask);
    if (pthread_sigmask(SIG_BLOCK, &mask, NULL) != 0)
    {
        printf("sigwait failed!\n");
        exit(-1);
    }

    pthread_t tid;
    pthread_create(&tid, NULL, thrFn, NULL);


    pthread_join(tid, NULL);

    return 0;
}
