// =======================================================================
//  FileName:     test_common.h
//  Author:       dingfang
//  CreateDate:   2020-12-14 20:08:20
//  ModifyAuthor: dingfang
//  ModifyDate:   2020-12-14 20:57:36
// =======================================================================

#ifndef __TEST_COMMON_H__
#define __TEST_COMMON_H__

#include <signal.h>
#include <stdio.h>

void pr_mask(const char *str)
{
    sigset_t sigset;

    if (sigprocmask(0, NULL, &sigset) < 0)
    {
        fprintf(stderr, "pr_mask: sigprocmask failed!\n");
        return ;
    }
    else
    {
        printf("%s", str);
        if (sigismember(&sigset, SIGINT))
        {
            printf(" SIGINT");
        }
        if (sigismember(&sigset, SIGQUIT))
        {
            printf(" SIGQUIT");
        }
        if (sigismember(&sigset, SIGALRM))
        {
            printf(" SIGALRM");
        }
        if (sigismember(&sigset, SIGUSR1))
        {
            printf(" SIGUSR1");
        }
    }

    printf("\n");
}


#endif /* __TEST_COMMON_H__ */
