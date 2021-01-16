// =======================================================================
//  FileName:     test_offsetof.c
//  Author:       dingfang
//  CreateDate:   2021-01-16 15:25:05
//  ModifyAuthor: dingfang
//  ModifyDate:   2021-01-16 15:30:18
// =======================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OFFSETOF(s,m) (size_t)(&(((s *)0)->m))

struct t1
{
    int m1;
    int m2;
    char m31;
    char m32;
    char m33;
    int m4;
};

int main(void)
{
    int len = OFFSETOF(struct t1, m32);
    printf("len: %d\n", len);

    return 0;
}
