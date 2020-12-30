// =======================================================================
//  FileName:     test_mandatory_lock.c
//  Author:       dingfang
//  CreateDate:   2020-12-30 18:45:30
//  ModifyAuthor: dingfang
//  ModifyDate:   2020-12-30 19:05:50
// =======================================================================


#include "test_lock_reg.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>


int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "usage: %s filename\n", argv[0]);
        exit(-1);
    }

    int fd;
    if ((fd = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0)
    {
        printf("open file failed!, errno: %d\n", errno);
        exit(-1);
    }

    if (write(fd, "abcdef", 6) != 6)
    {
        printf("write failed!\n");
        exit(-1);
    }

    struct stat statbuf;
    if (fstat(fd, &statbuf) < 0)
    {
        printf("fstat failed!\n");
        exit(-1);
    }

    if (fchmod(fd, (statbuf.st_mode & ~S_IXGRP) | S_ISGID) < 0)
    {
        printf("fchmod failed!\n");
        exit(-1);
    }

    TELL_WAIT();

    pid_t pid;
    if ((pid = fork()) < 0)
    {
        printf("fork failed!\n");
        exit(-1);
    }
    else if (pid > 0)
    {
        if (write_lock(fd, 0, SEEK_SET, 0) < 0)
        {
            printf("write_lock failed!\n");
            exit(-1);
        }

        TELL_CHILD(pid);

        if (waitpid(pid, NULL, 0) < 0)
        {
            printf("waitpid failed!\n");
            exit(-1);
        }
    }
    else
    {
        WAIT_PARENT();

        set_fl(fd, O_NONBLOCK);

        if (read_lock(fd, 0, SEEK_SET, 0) != -1)
        {
            printf("child: read_lock successful!\n");
        }
        printf("read_lock of already-locked region returns %d\n", errno);

        if (lseek(fd, 0, SEEK_SET) == -1)
        {
            printf("lseek failed!\n");
            exit(-1);
        }

        char buf[5];
        if (read(fd, buf, 2) < 0)
        {
            printf("read failed (mandatory locking works)!\n");
            exit(-1);
        }
        else
        {
            printf("read OK (no mandatory locking), buf = %2.2s\n", buf);
        }

    }



    return 0;
}
