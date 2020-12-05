// =======================================================================
//  FileName:     test_echoall.c
//  Author:       dingfang
//  CreateDate:   2020-12-05 12:16:56
//  ModifyAuthor: dingfang
//  ModifyDate:   2020-12-05 12:20:35
// =======================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    extern char **environ;
    printf("\n");
    for (int i = 0; i < argc; ++i)
    {
        printf("argv[%d]: %s\n", i, argv[i]);
    }

    for (char **ptr = environ; *ptr != 0; ++ptr)
    {
        printf("%s\n", *ptr);
    }

    return 0;
}
