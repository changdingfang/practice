// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// FileName:     test_resource.c
// Author:       dingfang
// CreateDate:   2020-12-01 19:34:49
// ModifyAuthor: dingfang
// ModifyDate:   2020-12-01 21:41:39
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#include <sys/resource.h>
#include <stdio.h>

#define doit(name) pr_limits(#name, name)

void pr_limits(char *name, int resource)
{
    struct rlimit lt;
    unsigned long long lim;

    if (getrlimit(resource, &lt) < 0)
    {
        fprintf(stderr, "getrlimit error for %s\n", name);
        return ;
    }

    printf("%-14s ", name);
    if (lt.rlim_cur == RLIM_INFINITY)
    {
        printf("(infinite) ");
    }
    else
    {
        lim = lt.rlim_cur;
        printf("%10lld ", lim);
    }

    if (lt.rlim_max == RLIM_INFINITY)
    {
        printf("(infinite)");
    }
    else
    {
        lim = lt.rlim_max;
        printf("%10lld", lim);
    }

    putchar('\n');
}

int main(void)
{
    doit(RLIMIT_CORE);
    doit(RLIMIT_CPU);
    doit(RLIMIT_DATA);
    doit(RLIMIT_FSIZE);

    doit(RLIMIT_MEMLOCK);
    doit(RLIMIT_MSGQUEUE);
    doit(RLIMIT_NOFILE);
    doit(RLIMIT_NPROC);
    doit(RLIMIT_RSS);
    doit(RLIMIT_SIGPENDING);
    doit(RLIMIT_STACK);

    return 0;
}
