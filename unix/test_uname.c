// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// FileName:     test_uname.c
// Author:       dingfang
// CreateDate:   2020-11-28 21:00:51
// ModifyAuthor: dingfang
// ModifyDate:   2020-11-28 21:08:30
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/utsname.h>

int main(void)
{
    struct utsname un;

    uname(&un);

    printf("sysname:\t%s\n", un.sysname);
    printf("nodename:\t%s\n", un.nodename);
    printf("release:\t%s\n", un.release);
    printf("version:\t%s\n", un.version);
    printf("machine:\t%s\n", un.machine);

    char name[128];
    gethostname(name, 128);

    printf("name:\t%s\n", name);


    return 0;
}
