// =======================================================================
//  FileName:     test_pthread_key.c
//  Author:       dingfang
//  CreateDate:   2020-12-22 21:28:30
//  ModifyAuthor: dingfang
//  ModifyDate:   2020-12-22 21:46:24
// =======================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>


static pthread_key_t key;
pthread_once_t initDone = PTHREAD_ONCE_INIT;

void myFree(void *arg)
{
    /* key的析构函数 */
    /* 在线程正常退出时会被调用 */
    free(arg);
    printf("my free\n");
    return ;
}

static void threadInit()
{
    printf("thread init\n");
    pthread_key_create(&key, myFree);
}

void *thrFn(void *arg)
{
    printf("thread func start\n");

    char *p = (char *)pthread_getspecific(key);

    if (p == NULL)
    {
        printf("p is null\n");
        p = (char *)malloc(20);
        strcpy(p, "hello world");

        printf("p = %s\n", p);
        pthread_setspecific(key, p);
    }


    printf("thread func end\n");
    pthread_exit((void *)0);
}


int main(void)
{
    pthread_once(&initDone, threadInit);
    pthread_t tid;

    pthread_create(&tid, NULL, thrFn, NULL);

    printf("main pthread join...\n");
    pthread_join(tid, NULL);

    sleep(3);

    return 0;
}
