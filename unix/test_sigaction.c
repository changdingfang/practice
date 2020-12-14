// =======================================================================
//  FileName:     test_sigaction.c
//  Author:       dingfang
//  CreateDate:   2020-12-14 18:54:07
//  ModifyAuthor: dingfang
//  ModifyDate:   2020-12-14 19:08:00
// =======================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

typedef void (*Sigfunc)(int);

Sigfunc mySignal(int signo, Sigfunc func)
{
    struct sigaction act, oact;

    act.sa_handler = func;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    if (signo == SIGALRM)
    {
#ifdef SA_INTERRUPT
        act.sa_flags |= SA_INTERRUPT;
#endif
    }
    else
    {
        act.sa_flags |= SA_RESTART;
    }

    if (sigaction(signo, &act, &oact) < 0)
    {
        return (SIG_ERR);
    }

    return oact.sa_handler;
}


static void sigAlrm(int signo)
{
    printf("sigalrm signo: %d\n", signo);
    return ;
}


int main(void)
{
    printf("main start\n");
    mySignal(SIGALRM, sigAlrm);
    alarm(3);
    // pause();
    char buf[30];
    int ret = read(STDIN_FILENO, buf, 30);
    printf("ret: %d\n", ret);

    printf("main end\n");
    return 0;
}
