// =======================================================================
//  FileName:     test_lock_test.c
//  Author:       dingfang
//  CreateDate:   2020-12-28 20:38:27
//  ModifyAuthor: dingfang
//  ModifyDate:   2020-12-28 21:49:41
// =======================================================================

#include "test_lock_reg.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>


static void lockabyte(const char *name, int fd, off_t offset)
{
    if (writew_lock(fd, offset, SEEK_SET, 1) < 0)
    {
        printf("%s: writew_lock error!\n", name);
        exit(-1);
    }
    printf("%s: got the lock, byte %lld\n", name, (long long)offset);
}


int main(void)
{
    int fd;
    if ((fd = creat("temp.lock", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0)
    {
        printf("create failed!\n");
        exit(-1);
    }

    if (write(fd, "ab", 2) != 2)
    {
        printf("write failed!\n");
        exit(-1);
    }

    TELL_WAIT();
    pid_t pid;
    if ((pid = fork()) < 0)
    {
        printf("fork failed!\n");
        exit(-1);
    }
    else if (pid == 0)
    {
        lockabyte("child", fd, 0);
        TELL_PARENT(getppid());
        WAIT_PARENT();
        lockabyte("child", fd, 1);
    }
    else
    {
        lockabyte("parent", fd, 1);
        TELL_CHILD(pid);
        WAIT_CHILD();
        lockabyte("parent", fd, 0);
    }

    return 0;
}


/*
 * 程序两个进程都尝试对0~1，和1~2进行加锁，导致出错
 * 0       1       2
 * +-------+-------+
 * |       |       |
 * +-------+-------+
 *
 * 可能的输出
 * child: got the lock, byte 0
 * child: writew_lock error!
 * parent: got the lock, byte 1
 * parent: got the lock, byte 0
 */
