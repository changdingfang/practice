// =======================================================================
//  FileName:     test_barrier.c
//  Author:       dingfang
//  CreateDate:   2020-12-19 11:23:32
//  ModifyAuthor: dingfang
//  ModifyDate:   2020-12-19 13:22:07
// =======================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <pthread.h>
#include <sys/time.h>

#define NTHR 8
#define NUMNUM 80000000L
#define TNUM (NUMNUM/NTHR)

long nums[NUMNUM];
long snums[NUMNUM];

pthread_barrier_t b;

int comlong(const void *arg1, const void *arg2)
{
    long l1 = *(long *)arg1;
    long l2 = *(long *)arg2;

    if (l1 == l2)
    {
        return 0;
    }
    return l1 < l2 ? -1 : 1;
}


void *thrFn(void *arg)
{
    long idx = (long)arg;

    qsort(&nums[idx], TNUM, sizeof(long), comlong);
    pthread_barrier_wait(&b);

    return ((void *)0);
}


void merge()
{
    long idx[NTHR];
    long minidx, num;

    for (int i = 0; i < NTHR; ++i)
    {
        idx[i] = i * TNUM;
    }

    for (int sidx = 0; sidx < NUMNUM; ++sidx)
    {
        num = LONG_MAX;
        for (int i = 0; i < NTHR; ++i)
        {
            /* 
             * idx[i] < (i + 1) * TNUM 是为了防止数组越界
             * (整条数组被分为NTHR个数组)
             * */
            if (idx[i] < (i +1) * TNUM && nums[idx[i]] < num)
            {
                num = nums[idx[i]];
                minidx = i;
            }
        }
        snums[sidx] = nums[idx[minidx]];
        idx[minidx]++;
    }
}


int main(void)
{
    struct timeval start, end;
    pthread_t tid;

    srandom(time(NULL));
    for (unsigned int i = 0; i < NUMNUM; ++i)
    {
        nums[i] = random();
    }

    gettimeofday(&start, NULL);
#define ONE
#ifdef ONE
    /* 在这里 NTHR 的个数和cpu核心数相当才能发挥出多线程的威力 */
    printf("NTHR\n");
    /* 
     * 只有达到设定的count数量的线程到达
     * pthread_barrier_wait后才会一同往下执行
     * 否则都会阻塞在pthread_barrier_wait处
     * 这里创建NTHR个线程,算上主线程, 一共NTHR + 1个
     * */
    pthread_barrier_init(&b, NULL, NTHR + 1);
    for (unsigned int i = 0; i < NTHR; ++i)
    {
        if (pthread_create(&tid, NULL, thrFn, (void *)(i * TNUM)) != 0)
        {
            fprintf(stderr, "can't create thread\n");
            exit(-1);
        }
    }
    pthread_barrier_wait(&b);

    merge();
#else
    printf("one \n");
    qsort(&nums[0], NUMNUM, sizeof(long), comlong);
#endif
    gettimeofday(&end, NULL);

    printf("sort tok %.4f seconds\n",
            (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_usec - start.tv_usec) / 1000.0
            );

    return 0;
}
