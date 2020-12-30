// =======================================================================
//  FileName:     test_select.c
//  Author:       dingfang
//  CreateDate:   2020-12-30 20:06:12
//  ModifyAuthor: dingfang
//  ModifyDate:   2020-12-30 20:10:04
// =======================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>
#include <sys/time.h>

int main(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    tv.tv_sec = 2;
    tv.tv_usec = 125125;
    select(10, NULL, NULL, NULL, &tv);

    return 0;
}
