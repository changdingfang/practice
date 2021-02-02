//***************************************************************
// @file:    heap.c
// @author:  dingfang
// @date    2018-11-15 17:29:31
//***************************************************************

/* 堆排序 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define LEN 1000000


#define swap(a, b) { int c = a; a = b; b = c; }
/* #define swap(a, b) { a = a ^ b; b = b ^ a; a = a ^ b; } */


unsigned long calTime(struct timeval v1, struct timeval v2)
{
    return ((v2.tv_sec - v1.tv_sec) * 1000 + (v2.tv_usec - v1.tv_usec) / 1000);
}


void check(int *arr)
{
    int i = 0;
    for (i = 0; i < LEN - 1; ++i)
    {
        if (arr[i] > arr[i + 1])
        {
            printf("head sort error!\n");
            break;
        }
    }
}

void maxHeapify(int *a, int begin, int end)
{
    int dad = begin;
    int son = (dad << 1) + 1;

    while (son <= end)
    {
        if (son + 1 <= end && a[son] < a[son + 1])
        {
            ++son;
        }
        if (a[dad] > a[son])
        {
            return;
        }
        else
        {
            swap(a[dad], a[son]);
            dad = son;
            son = (son << 1) + 1;
        }
    }
}

void heapSort(int *arr, int len)
{
    int i;
    for (i = (len >> 1) - 1; i >= 0; --i)
    {
        maxHeapify(arr, i, len - 1);
    }

    for (i = len - 1; i > 0; --i)
    {
        swap(arr[0], arr[i]);
        maxHeapify(arr, 0, i - 1);
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
        if (count++ > 5)
            break;
        for (i = 0; i < LEN; ++i)
        {
            arr[i] = rand() % 10000;
        }

        struct timeval v1;
        gettimeofday(&v1, NULL);
        heapSort(arr, LEN);
        struct timeval v2;
        gettimeofday(&v2, NULL);
        printf("use time: %ld\n", calTime(v1, v2));

        check(arr);
    } while (1);

    return 0;
}

