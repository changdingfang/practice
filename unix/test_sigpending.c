// =======================================================================
//  FileName:     test_sigpending.c
//  Author:       dingfang
//  CreateDate:   2020-12-13 12:40:58
//  ModifyAuthor: dingfang
//  ModifyDate:   2020-12-13 12:48:43
// =======================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

static void sigQuit(int signo)
{
    printf("caught SIGQUIT\n");
    if (signal(SIGQUIT, SIG_DFL) == SIG_ERR)
    {
        fprintf(stderr, "signal error!\n");
        exit(-1);
    }
}


int main(void)
{
    sigset_t newmask, oldmask, pendmask;

    if (signal(SIGQUIT, sigQuit) == SIG_ERR)
    {
        fprintf(stderr, "signal SIGQUIT error!\n");
        exit(-1);
    }

    sigemptyset(&newmask);
    sigaddset(&newmask, SIGQUIT);

    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
    {
        fprintf(stderr, "signal error!\n");
        exit(-1);
    }
    sleep(5);

    if (sigpending(&pendmask) < 0)
    {
        fprintf(stderr, "signal error!\n");
        exit(-1);
    }
    if (sigismember(&pendmask, SIGQUIT))
    {
        printf("\nSIGQUIT pending\n");
    }

    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
    {
        fprintf(stderr, "sigprocmask error!\n");
        exit(-1);
    }
    printf("SIGQUIT, unblocked\n");

    sleep(5);

    return 0;
}
