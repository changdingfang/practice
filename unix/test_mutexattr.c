// =======================================================================
//  FileName:     test_mutexattr.c
//  Author:       dingfang
//  CreateDate:   2020-12-21 20:49:20
//  ModifyAuthor: dingfang
//  ModifyDate:   2020-12-21 20:54:01
// =======================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>


int main(void)
{
    pthread_mutexattr_t attr;

    pthread_mutexattr_init(&attr);

    int pshared = -1;
    /* 获取进程间共享标志,查看是否已设置该标志位  */
    pthread_mutexattr_getpshared(&attr, &pshared);

    printf("PRIVATE = %d\n", PTHREAD_PROCESS_PRIVATE);
    printf("SHARED  = %d\n", PTHREAD_PROCESS_SHARED);
    printf("pshared = %d\n", pshared);

    pthread_mutexattr_destroy(&attr);

    return 0;
}
