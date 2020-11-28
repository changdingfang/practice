// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// FileName:     test_setbuf.c
// Author:       dingfang
// CreateDate:   2020-11-28 16:42:17
// ModifyAuthor: dingfang
// ModifyDate:   2020-11-28 17:20:59
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

/*
 * unix 环境高级编程 第五章习题第一题
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SIZE 1024

void mySetbuf(FILE *fp, char *buf)
{
    /* 使用setvbuf实现setbuf */


    if (buf == NULL)
    {
        setvbuf(fp, buf, _IONBF, BUFSIZ);
        return ;
    }

    int fd = fileno(fp);
    if (fd < 0)
    {
        fprintf(stderr, "FILE fp error!\n");
        exit(-1);
    }

    setvbuf(fp, buf, isatty(fd) ? _IOLBF : _IOFBF, BUFSIZ);
}

void testMysetbuf()
{
    char buf[BUFSIZ];

    mySetbuf(stdout, buf);

    fprintf(stdout, "hello \nworld!");

    sleep(20);
}

void testSetbuf()
{
    /* 不带缓冲,直接输出 */
    setbuf(stdout, NULL);
    fprintf(stdout, "hello world");
    // sleep(5);

    /* 带有缓冲 */
    char buf[BUFSIZ];
    setbuf(stdout, buf);
    fprintf(stdout, "hello \nworld");

    sleep(30);
}

void testSetvbuf()
{
    char buf[SIZE];

#if 1
    /* 不带缓冲,全部输出 */
    setvbuf(stdout, buf, _IONBF, SIZE);

#elif 1
    /* 行缓冲, 程序结束前输出 "hello" */
    setvbuf(stdout, buf, _IOLBF, SIZE);
#else
    /* 全缓冲, 程序结束前什么都不输出 */
    setvbuf(stdout, buf, _IOFBF, SIZE);
#endif
    fprintf(stdout, "hello \nworld!");

    sleep(20);
}

int main(void)
{
    testSetbuf();
    // testSetvbuf();

    // testMysetbuf();
    return 0;
}
