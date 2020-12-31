// =======================================================================
//  FileName:     test_mmap.c
//  Author:       dingfang
//  CreateDate:   2020-12-31 21:48:09
//  ModifyAuthor: dingfang
//  ModifyDate:   2020-12-31 21:57:36
// =======================================================================

#include "test_lock_reg.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define COPYINCR (1024 * 1024 *1024)


int main(int argc, char ** argv)
{
    if (argc != 3)
    {
        printf("usage: %s <fromfile> <tofile>\n", argv[0]);
        exit(-1);
    }

    int fdin, fdout;
    if ((fdin = open(argv[1], O_RDONLY)) < 0)
    {
        printf("open failed!, filename: %s\n", argv[1]);
        exit(-1);
    }

    if ((fdout = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, FILE_MODE)) < 0)
    {
        printf("open failed!, filename: %s\n", argv[2]);
        exit(-1);
    }

    struct stat sbuf;
    if (fstat(fdin, &sbuf) < 0)
    {
        printf("fstat failed!\n");
        exit(-1);
    }

    if (ftruncate(fdout, sbuf.st_size) < 0)
    {
        printf("ftruncate failed!, size: %ld\n", sbuf.st_size);
        exit(-1);
    }

    size_t copysz;
    off_t fsz  = 0;
    void *src, *dst;
    while (fsz < sbuf.st_size)
    {
        if ((sbuf.st_size - fsz) > COPYINCR)
        {
            copysz = COPYINCR;
        }
        else
        {
            copysz = sbuf.st_size - fsz;
        }

        if ((src = mmap(NULL, copysz, PROT_READ, MAP_SHARED, fdin, fsz)) == MAP_FAILED)
        {
            printf("mmap failed! --- in\n");
            exit(-1);
        }

        if ((dst = mmap(NULL, copysz, PROT_READ | PROT_WRITE, MAP_SHARED, fdout, fsz)) == MAP_FAILED)
        {
            printf("mmap failed! --- out\n");
            exit(-1);
        }

        memcpy(dst, src, copysz);
        munmap(src, copysz);
        munmap(dst, copysz);

        fsz += copysz;
    }

    return 0;
}
