// =======================================================================
//  FileName:     test_getpass.c
//  Author:       dingfang
//  CreateDate:   2021-01-20 08:43:09
//  ModifyAuthor: dingfang
//  ModifyDate:   2021-01-20 08:48:27
// =======================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
    char *ptr;
    /* 在终端输入时没有回显，适用于密码输入 */
    if ((ptr = getpass("Enter: ")) == NULL)
    {
        printf("getpass failed!\n");
        exit(-1);
    }

    printf("output: %s\n", ptr);

    /* 清楚该部分内存，为了安全 */
    while (*ptr != 0)
    {
        *ptr++ = 0;
    }

    return 0;
}
