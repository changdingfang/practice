// =======================================================================
//  FileName:     test_dev_zero.c
//  Author:       dingfang
//  CreateDate:   2021-01-09 20:35:18
//  ModifyAuthor: dingfang
//  ModifyDate:   2021-01-09 20:51:32
// =======================================================================

#include "test_lock_reg.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#define NLOOPS 1000
#define SIZE sizeof(long)

static int update(long *ptr)
{
    return ((*ptr)++);
}

int main(void)
{
    int fd, counter;
    pid_t pid;
    void *area;

    if ((fd = open("/dev/zero", O_RDWR)) < 0)
    {
        printf("open failed!\n");
        exit(-1);
    }

    if ((area = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED)
    {
        printf("mmap failed!\n");
        exit(-1);
    }

    close(fd);

    TELL_WAIT();

    if ((pid = fork()) < 0)
    {
        printf("fork failed!\n");
        exit(-1);
    }
    else if (pid > 0)
    {
        for (int i = 0; i < NLOOPS; i += 2)
        {
            if ((counter = update((long *)area)) != i)
            {
                printf("parent: update failed i = %d, area = %ld!\n", i, *(long *)area);
                exit(-1);
            }
            TELL_CHILD(pid);
            WAIT_CHILD();
        }
    }
    else
    {
        for (int i = 1; i < NLOOPS + 1; i += 2)
        {
            WAIT_PARENT();
            if ((counter = update((long *)area)) != i)
            {
                printf("child: update failed i = %d, area = %ld!\n", i, *(long *)area);
                exit(-1);
            }

            TELL_PARENT(getppid());
        }
    }

    printf("area = %ld\n", *(long *)area);

    return 0;
}
