// =======================================================================
//  FileName:     test_popen.c
//  Author:       dingfang
//  CreateDate:   2021-01-12 19:50:44
//  ModifyAuthor: dingfang
//  ModifyDate:   2021-01-12 21:34:27
// =======================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    /* popen会创建一个子进程，执行命令，输出或写到管道中 */
    FILE *fp = popen("echo hello world", "r");

    char buf[1024];
    fread(buf, 1, 1024, fp);

    printf("buf: %s\n", buf);

    pclose(fp);

    return 0;
}
