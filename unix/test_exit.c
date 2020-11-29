// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// FileName:     test_exit.c
// Author:       dingfang
// CreateDate:   2020-11-29 16:46:08
// ModifyAuthor: dingfang
// ModifyDate:   2020-11-29 16:47:16
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void exit1()
{
    printf("exit 1\n");
}

void exit2()
{
    printf("exit 2\n");
}

int main(void)
{
    atexit(exit1);
    atexit(exit1);
    atexit(exit2);

    printf("main exit\n");
    return 0;
}
