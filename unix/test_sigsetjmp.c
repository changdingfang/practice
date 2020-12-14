// =======================================================================
//  FileName:     test_sigsetjmp.c
//  Author:       dingfang
//  CreateDate:   2020-12-14 18:28:57
//  ModifyAuthor: dingfang
//  ModifyDate:   2020-12-14 18:43:53
// =======================================================================

/*
 * 在信号处理函数中，会自动屏蔽当前处理的信号；比如正在处理SIGINT信号，
 * 那么在处理过程中，再有SIGINT信号，也会被丢弃
 *
 * 这就导致一个问题
 * 信号处理函数中进行跳转，使用setjmp和longjmp将导致信号屏蔽无法取消，
 * 从而导致跳出信号处理函数后(非正常退出)，再有相同的信号，也无法处理
 * 而是被丢弃掉
 * 所以这个时候应当使用sigsetjmp和siglongjmp成对函数，可以避免这种情况
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <signal.h>

#define SIG

#ifdef SIG
sigjmp_buf sigjmpbuf;
#else
jmp_buf jmpbuf;
#endif

static void sigInt(int signo)
{
    printf("SIGINT start\n");
#ifdef SIG
    siglongjmp(sigjmpbuf, 0);
#else
    longjmp(jmpbuf, 0);
#endif
}


int main(void)
{
    signal(SIGINT, sigInt);

#ifdef SIG
    sigsetjmp(sigjmpbuf, 1);
#else
    setjmp(jmpbuf);
#endif

    printf("setjmp over\n");

    for ( ; ; )
    {
    }

    printf("main end\n");

    return 0;
}
