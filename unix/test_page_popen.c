// =======================================================================
//  FileName:     test_page_popen.c
//  Author:       dingfang
//  CreateDate:   2021-01-04 20:53:18
//  ModifyAuthor: dingfang
//  ModifyDate:   2021-01-04 21:00:42
// =======================================================================

/* 使用 popen 重写 test_page_pipe.c 程序 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#define PAGER "${PAGER:-more}"
#define MAXLINE 2048

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("usage: %s <pathname>", argv[0]);
        return -1;
    }

    FILE *fpin, *fpout;
    if ((fpin = fopen(argv[1], "r")) == NULL)
    {
        printf("can't open %s\n", argv[1]);
        exit(-1);
    }

    /* 新进程的可写输入描述符 */
    if ((fpout = popen(PAGER, "w")) == NULL)
    {
        printf("popen  failed!\n");
        exit(-1);
    }

    char line[MAXLINE];
    while (fgets(line, MAXLINE, fpin) != NULL)
    {
        if (fputs(line, fpout) == EOF)
        {
            printf("fputs failed!\n");
            exit(-1);
        }
    }

    if (ferror(fpin))
    {
        printf("fgets failed!\n");
        exit(-1);
    }

    if (pclose(fpout) == -1)
    {
        printf("pclose failed!\n");
        exit(-1);
    }

    return 0;
}
