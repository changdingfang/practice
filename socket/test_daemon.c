// =======================================================================
//  FileName:     test_daemon.c
//  Author:       dingfang
//  CreateDate:   2021-03-30 07:32:37
//  ModifyAuthor: dingfang
//  ModifyDate:   2021-03-30 07:36:42
// =======================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    printf("hello wrold 0000 \n");
    daemon(1, 0);

    printf("hello wrold\n");
    while (1) 
    {
        sleep(1);
    };

    return 0;
}
