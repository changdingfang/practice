// =======================================================================
//  FileName:     test_pthread_sigmask.c
//  Author:       dingfang
//  CreateDate:   2020-12-23 18:56:39
//  ModifyAuthor: dingfang
//  ModifyDate:   2020-12-23 19:17:30
// =======================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>


void sigFn(int signo)
{
    printf("signal func; thread id = %ld\n", pthread_self());
    printf("signal func\n");
    return ;
}


void *thrFn1(void *arg)
{
#if 0
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGALRM);
    pthread_sigmask(SIG_BLOCK, &set, NULL);
#endif

    signal(SIGINT, sigFn);
    printf("thread func 1 thread id = %ld\n", pthread_self());
    while (1)
    {
        sleep(1);
    }
    pthread_exit((void *)0);
}


void *thrFn2(void *arg)
{
    printf("thread func 2 thread id = %ld\n", pthread_self());
    pthread_t tid = *(pthread_t *)arg;

    printf("thread func 2 thread id2 = %ld\n", tid);
    while (1)
    {
        sleep(2);
        pthread_kill(tid, SIGINT);
    }

    pthread_exit((void *)0);
}

int main(void)
{

    signal(SIGINT, sigFn);
    signal(SIGALRM, sigFn);

#if 1
    /* 创建线程之前使用屏蔽，其它线程共享 */
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGALRM);
    pthread_sigmask(SIG_BLOCK, &set, NULL);
#endif

    printf("main thread thread id = %ld\n", pthread_self());
    pthread_t tid1, tid2;
    pthread_create(&tid1, NULL, thrFn1, NULL);
    pthread_create(&tid2, NULL, thrFn2, &tid1);

#if 0
    /* 创建线程之后屏蔽信号，仅对该线程生效 */
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGALRM);
    pthread_sigmask(SIG_BLOCK, &set, NULL);
#endif


    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    return 0;
}
