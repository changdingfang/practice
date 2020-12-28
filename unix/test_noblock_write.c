// =======================================================================
//  FileName:     test_noblock_write.c
//  Author:       dingfang
//  CreateDate:   2020-12-28 20:00:02
//  ModifyAuthor: dingfang
//  ModifyDate:   2020-12-28 20:08:16
// =======================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

int main(void)
{
    char buf[500000];

    int nwrite, ntowrite;
    char *ptr;

    ntowrite = read(STDIN_FILENO, buf, sizeof(buf));
    fprintf(stderr, "read %d bytes\n", ntowrite);

    int val;
    if ((val = fcntl(STDOUT_FILENO, F_GETFL, 0)) < 0)
    {
        printf("fcntl get failed!\n");
        exit(-1);
    }
    val |= O_NONBLOCK;
    if (fcntl(STDOUT_FILENO, F_SETFL, val) < 0)
    {
        printf("fcntl set failed!\n");
        exit(-1);
    }

    ptr = buf;

    while (ntowrite > 0)
    {
        nwrite = write(STDOUT_FILENO, ptr, ntowrite);
        fprintf(stderr, "nwrite = %d, errno = %d\n", nwrite, errno);

        if (nwrite > 0)
        {
            ptr += nwrite;
            ntowrite -= nwrite;
        }
    }

    return 0;
}
