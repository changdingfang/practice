// =======================================================================
//  FileName:     test_pthread_1.c
//  Author:       dingfang
//  CreateDate:   2020-12-19 11:03:43
//  ModifyAuthor: dingfang
//  ModifyDate:   2020-12-19 11:21:46
// =======================================================================

/*
 * 第11章习题 11.1 修改后的11-4实例代码
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

struct Foo_T
{
    int a;
    int b;
    int c;
    int d;
};


void printfoo(const char *s, const struct Foo_T *fp)
{
    printf("%s", s);
    printf(" structure at 0x%lx\n", (unsigned long)fp);
    printf(" foo.a = %d\n", fp->a);
    printf(" foo.b = %d\n", fp->b);
    printf(" foo.c = %d\n", fp->c);
    printf(" foo.d = %d\n", fp->d);
}


void *thrFn1(void *arg)
{
    struct Foo_T *pFoo = (struct Foo_T *)arg;

    printfoo("thread 1: \n", pFoo);
    pthread_exit((void *)pFoo);
}


void *thrFn2(void *arg)
{
    printf("thread 2: ID is %lu\n", (unsigned long)pthread_self());
    pthread_exit((void *)0);
}


int main(void)
{
    int err;
    pthread_t tid1, tid2;
    struct Foo_T *fp;

    struct Foo_T foo = {1, 2, 3, 4};
    err = pthread_create(&tid1, NULL, thrFn1, &foo);
    if (err != 0)
    {
        fprintf(stderr, "can't create thread 1\n");
        exit(-1);
    }
    err = pthread_join(tid1, (void *)&fp);
    if (err != 0)
    {
        fprintf(stderr, "can't join with thread 1");
        exit(-1);
    }
    sleep(1);

    printf("parent starting second thread\n");

    err = pthread_create(&tid2, NULL, thrFn2, NULL);
    if (err != 0)
    {
        fprintf(stderr, "can't create thread 2\n");
        exit(-1);
    }

    err = pthread_join(tid2, NULL);
    if (err != 0)
    {
        fprintf(stderr, "can't join with thread 2");
        exit(-1);
    }
    sleep(1);
    printfoo("parent: \n", fp);
    return 0;
}
