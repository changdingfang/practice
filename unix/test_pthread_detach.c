// =======================================================================
//  FileName:     test_pthread_detach.c
//  Author:       dingfang
//  CreateDate:   2020-12-19 18:13:48
//  ModifyAuthor: dingfang
//  ModifyDate:   2020-12-19 19:54:13
// =======================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>


void *thrFn(void *arg)
{
    printf("pthread ...\n");

    pthread_exit((void *)0);
}


int main(void)
{
    pthread_t tid;


    pthread_attr_t attr;
    pthread_attr_init(&attr);
    // pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    // 未设置情况下, 默认为JOINABLE
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    int mode;
    pthread_attr_getdetachstate(&attr, &mode);
    printf("JOIN: %d\n", PTHREAD_CREATE_JOINABLE);
    printf("DETACH: %d\n", PTHREAD_CREATE_DETACHED);
    printf("mode = %d\n", mode);

    size_t ssize;
    pthread_attr_getstacksize(&attr, &ssize);
    printf("stack size= %lu\n", ssize);

    pthread_create(&tid, &attr, thrFn, NULL);
    /* 线程分离下, 使用join回收会返回错误 */
    int err = pthread_join(tid, NULL);
    printf("err = %d\n", err);

    pthread_attr_destroy(&attr);

    return 0;
}
