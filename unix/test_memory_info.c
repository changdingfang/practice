// =======================================================================
//  FileName:     test_memory_info.c
//  Author:       dingfang
//  CreateDate:   2021-01-09 20:01:30
//  ModifyAuthor: dingfang
//  ModifyDate:   2021-01-09 20:32:31
// =======================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>

#define ARRAY_SIZE  40000
#define MALLOC_SIZE 100000
#define SHM_SIZE    100000
#define SHM_MODE    0600

char array[ARRAY_SIZE];

int main(void)
{
    int shmid;
    char *ptr, *shmptr;
    printf("array[] form\t%p to %p\n", (void *)&array[0], (void *)&array[ARRAY_SIZE]);
    printf("stack around\t%p\n", (void *)&shmid);

    if ((ptr = malloc(MALLOC_SIZE)) == NULL)
    {
        fprintf(stderr, "malloc failed!\n");
        return -1;
    }

    printf("malloced from\t%p to %p\n", (void *)ptr, (void *)ptr + MALLOC_SIZE);

    if ((shmid = shmget(IPC_PRIVATE, SHM_SIZE, SHM_MODE)) < 0)
    {
        fprintf(stderr, "shmget failed!\n");
        return -1;
    }

    if ((shmptr = shmat(shmid, 0, 0)) == (void *)-1)
    {
        fprintf(stderr, "shmat failed!\n");
        exit(-1);
    }

    printf("shared memory attached from\t%p to %p\n", (void *)shmptr, (void *)shmptr + SHM_SIZE);

    if (shmctl(shmid, IPC_RMID, 0) < 0)
    {
        printf("shmctl failed!\n");
        exit(-1);
    }

    return 0;
}

/*

 <<-- 某次输出胡打印结果与内存堆栈信息分布对应关系 -->>
    
高地址  +-------------+
        |             |   命令行参数和环境变量
        +-------------+
        |             |
        |     栈      | <--- 0x7fffe65d3cb4
        |             |
        +-------------+
        |             | <--- 0x7fd9665236a0
        |  共享内存   |
        |             | <--- 0x7fd96650b000
        +-------------+
        |             | <--- 0x1266010
        |             |
        |     堆      |
        |             | <--- 0x127e6b0
        |             |
        +-------------+
        |             | <--- 0x60ace0
        |未初始化数据 |
        |             | <--- 0x6010a0
        +-------------+
        |已初始化数据 |
        +-------------+
        |     正文    |
低地址  +-------------+

*/
