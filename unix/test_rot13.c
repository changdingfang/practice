// =======================================================================
//  FileName:     test_rot13.c
//  Author:       dingfang
//  CreateDate:   2020-12-31 08:35:13
//  ModifyAuthor: dingfang
//  ModifyDate:   2020-12-31 20:46:50
// =======================================================================

#include "test_lock_reg.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>
#include <aio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>

#define BSZ  1024
#define NBUF 2


enum rwop
{
    UNUSED = 0,
    READ_PENDING = 1,
    WRITE_PENDING = 2
};


struct buf 
{
    enum rwop   op;
    int         last;
    struct aiocb aiocb;
    unsigned char data[BSZ];
};


struct buf bufs[NBUF];

unsigned char translate(unsigned char c)
{
    return c;
}


int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("usage:rot13 infile outfile\n");
        exit(-1);
    }

    int ifd, ofd;
    if ((ifd = open(argv[1], O_RDONLY)) < 0)
    {
        printf("open ifd failed! filename: %s\n", argv[1]);
        exit(-1);
    }

    if ((ofd = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, FILE_MODE)) < 0)
    {
        printf("open ofd failed! filename: %s\n", argv[2]);
        exit(-1);
    }

    struct stat sbuf;
    if (fstat(ifd, &sbuf) < 0)
    {
        printf("fstat failed\n");
        exit(-1);
    }

    const struct aiocb *aiolist[NBUF];

    for (int i = 0; i < NBUF; ++i)
    {
        bufs[i].op = UNUSED;
        bufs[i].aiocb.aio_buf = bufs[i].data;
        bufs[i].aiocb.aio_sigevent.sigev_notify = SIGEV_NONE;
        aiolist[i] = NULL;
    }


    off_t off = 0;
    int numop = 0;
    int err, n;

    for ( ;; )
    {
        for (int i = 0; i < NBUF; ++i)
        {
            switch (bufs[i].op)
            {
                case UNUSED:
                    {
                        if (off < sbuf.st_size)
                        {
                            bufs[i].op = READ_PENDING;
                            bufs[i].aiocb.aio_fildes = ifd;
                            bufs[i].aiocb.aio_offset = off;
                            off += BSZ;
                            if (off >= sbuf.st_size)
                            {
                                bufs[i].last = 1;
                            }
                            bufs[i].aiocb.aio_nbytes = BSZ;
                            if (aio_read(&bufs[i].aiocb) < 0)
                            {
                                printf("aio_read failed!\n");
                                exit(-1);
                            }
                            aiolist[i] = &bufs[i].aiocb;
                            ++numop;
                        }
                        break;
                    }
                case READ_PENDING:
                    {
                        if ((err = aio_error(&bufs[i].aiocb)) == EINPROGRESS)
                        {
                            continue;
                        }
                        if (err != 0)
                        {
                            if (err == -1)
                            {
                                printf("aio_error failed!\n");
                            }
                            else
                            {
                                printf("read failed!\n");
                                exit(-1);
                            }
                        }
                        if ((n = aio_return(&bufs[i].aiocb)) < 0)
                        {
                            printf("aio_return failed!\n");
                            exit(-1);
                        }
                        if (n != BSZ && !bufs[i].last)
                        {
                            printf("short read (%d/%d), last: %d\n", n, BSZ, bufs[i].last);
                            exit(-1);
                        }

                        for (int j = 0; j < n; ++j)
                        {
                            bufs[i].data[j] = translate(bufs[i].data[j]);
                        }
                        bufs[i].op = WRITE_PENDING;
                        bufs[i].aiocb.aio_fildes = ofd;
                        bufs[i].aiocb.aio_nbytes = n;
                        if (aio_write(&bufs[i].aiocb) < 0)
                        {
                            printf("aio_write failed!\n");
                            exit(-1);
                        }
                        break;
                    }
                case WRITE_PENDING:
                    {
                        if ((err = aio_error(&bufs[i].aiocb)) == EINPROGRESS)
                        {
                            continue;
                        }
                        if (err != 0)
                        {
                            if (err == -1)
                            {
                                printf("aio_error failed!\n");
                                /* exit(-1); */
                            }
                            else
                            {
                                printf("write failed\n");
                                exit(-1);
                            }
                        }
                        if ((n = aio_return(&bufs[i].aiocb)) < 0)
                        {
                            printf("aio_return failed!\n");
                            exit(-1);
                        }
                        if (n != bufs[i].aiocb.aio_nbytes)
                        {
                            printf("short write (%d/%d)\n", n, BSZ);
                            exit(-1);
                        }
                        aiolist[i] = NULL;
                        bufs[i].op = UNUSED;
                        --numop;
                        break;
                    }
            }
        }
        if (numop == 0)
        {
            if (off >= sbuf.st_size)
            {
                break;
            }
        }
        else
        {
            if (aio_suspend(aiolist, NBUF, NULL) < 0)
            {
                printf("aio_suspend failed!\n");
                exit(-1);
            }
        }
    }

    bufs[0].aiocb.aio_fildes = ofd;
    if (aio_fsync(O_SYNC, &bufs[0].aiocb) < 0)
    {
        printf("aio_fsync failed!\n");
        exit(-1);
    }

    return 0;
}
