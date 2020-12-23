// =======================================================================
//  FileName:     test_atfork.c
//  Author:       dingfang
//  CreateDate:   2020-12-23 20:13:08
//  ModifyAuthor: dingfang
//  ModifyDate:   2020-12-23 20:28:05
// =======================================================================

/* 
 * 把输出结果重定向到文件中，将会有和终端不一样的打印输出 
 * 原因是行缓冲和全缓冲
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;


void prepare(void)
{
    printf("preparing locks...\n");
    if (pthread_mutex_lock(&lock1) != 0)
    {
        printf("parent lock1 error!\n");
        exit(-1);
    }
    if (pthread_mutex_lock(&lock2) != 0)
    {
        printf("parent lock2 error!\n");
        exit(-1);
    }
}


void parent(void)
{
    printf("parent unlocking locks...\n");
    if (pthread_mutex_unlock(&lock1) != 0)
    {
        printf("parent unlock1 error!\n");
        exit(-1);
    }
    if (pthread_mutex_unlock(&lock2) != 0)
    {
        printf("parent unlock2 error!\n");
        exit(-1);
    }
}


void child(void)
{
    printf("child unlocking locks...\n");
    if (pthread_mutex_unlock(&lock1) != 0)
    {
        printf("child unlock1 error!\n");
        exit(-1);
    }
    if (pthread_mutex_unlock(&lock2) != 0)
    {
        printf("child unlock2 error!\n");
        exit(-1);
    }
}


void *thrFn(void *arg)
{
    printf("thread start ... \n");
    pause();

    return NULL;
}


int main(void)
{
    pid_t pid;
    pthread_t tid;

    if (pthread_atfork(prepare, parent, child) != 0)
    {
        printf("can't install fork handlers\n");
        exit(-1);
    }

    if (pthread_create(&tid, NULL, thrFn, NULL) != 0)
    {
        printf("cteate thread error!\n");
        exit(-1);
    }

    sleep(2);
    printf("parent about to fork...\n");
    if ((pid = fork()) < 0)
    {
        printf("cteate thread error!\n");
        exit(-1);
    }
    else if (pid == 0)
    {
        printf("child returned from fork\n");
    }
    else
    {
        printf("parent returned from fokr\n");
    }
    return 0;
}
