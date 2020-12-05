// =======================================================================
//  FileName:     test_getlogin.c
//  Author:       dingfang
//  CreateDate:   2020-12-05 15:20:52
//  ModifyAuthor: dingfang
//  ModifyDate:   2020-12-05 15:22:38
// =======================================================================

/*
 * 获取的登录时的用户名, 即使使用 su 命令切换用户, 打印的仍然是登录时的用户名
 * */
#include <stdio.h>
#include <unistd.h>

int main(void)
{
    printf("login name: [%s]\n", getlogin());

    return 0;
}
