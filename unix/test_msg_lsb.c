// =======================================================================
//  FileName:     test_msg_lsb.c
//  Author:       dingfang
//  CreateDate:   2021-01-14 22:12:49
//  ModifyAuthor: dingfang
//  ModifyDate:   2021-01-14 22:14:25
// =======================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    int n = 0x04030201;
    char *p = (char *)&n;

    if (*p == 4)
    {
        printf("big\n");
    }
    else if (*p == 1)
    {
        printf("little\n");
    }
    else
    {
        printf("unknow\n");
    }

    return 0;
}
