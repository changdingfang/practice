// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// FileName:     test_strftime.c
// Author:       dingfang
// CreateDate:   2020-11-28 21:46:31
// ModifyAuthor: dingfang
// ModifyDate:   2020-11-28 21:56:50
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

/* unix高级环境编程 第六章低5题 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(void)
{
    time_t t = time(NULL);

    char buf[64] = { 0 };
    struct tm tm;
    localtime_r(&t, &tm);
    strftime(buf, 64, "%a %b %d %T %Z %Y", &tm);

    printf("%s\n", buf);

    return 0;
}
