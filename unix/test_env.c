// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// FileName:     test_env.c
// Author:       dingfang
// CreateDate:   2020-11-29 16:49:33
// ModifyAuthor: dingfang
// ModifyDate:   2020-11-29 16:56:21
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#include <stdio.h>

void envprint()
{
    extern char **environ;

    char **p = environ;
    while (*p != NULL)
    {
        printf("%s\n", *p);
        ++p;
    }
}

int main(void)
{
    envprint();

    return 0;
}
