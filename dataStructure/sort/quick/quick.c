//***************************************************************
// @file:    quick.c
// @author:  dingfang
// @date    2018-11-14 17:05:35
//***************************************************************

/* 快排 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define LEN 300000

unsigned long calTime(struct timeval v1, struct timeval v2)
{
    return ((v2.tv_sec - v1.tv_sec) * 1000 + (v2.tv_usec - v1.tv_usec) / 1000);
}


void sortQuick(int *a, int l, int r)
{
    if (l >= r)
    {
        return;
    }
    int k = a[l];
    int i = l;
    int j = r;
    while (i < j)
    {
        while (i < j && k <= a[j])
        {
            --j;
        }
        a[i] = a[j];
        while (i < j && k >= a[i])
        {
            ++i;
        }
        a[j] = a[i];
    }
    a[i] = k;
    sortQuick(a, l, i - 1);
    sortQuick(a, i + 1, r);
}

void check(int *arr)
{
    int i = 0;
    for (i = 0; i < LEN - 1; ++i)
    {
        if (arr[i] > arr[i + 1])
        {
            printf("sort error\n");
            break;
        }
    }

    if (i == LEN - 1)
    {
        printf("successful\n");
    }
}

int main(void)
{
    int arr[LEN] = {0};

    int i, j;
    int count = 0;
    srand(time(NULL));
    do
    {
        if (count++ > 15)
            break;

        for (i = 0; i < LEN; ++i)
        {
            arr[i] = rand() % 100;
        }

        struct timeval v1;
        gettimeofday(&v1, NULL);
        sortQuick(arr, 0, LEN - 1);
        struct timeval v2;
        gettimeofday(&v2, NULL);
        printf("use time: %ld\n", calTime(v1, v2));

        check(arr);
    } while (1);

    return 0;
}

