//***************************************************************
// @file:    nonRecursiveQuick.c
// @author:  dingfang
// @date    2018-11-14 17:05:35
//***************************************************************

/* 快排--非递归 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <sys/time.h>

#define LEN 200000


unsigned long calTime(struct timeval v1, struct timeval v2)
{
    return ((v2.tv_sec - v1.tv_sec) * 1000 + (v2.tv_usec - v1.tv_usec) / 1000);
}


typedef struct Stack
{
    int *data_;
    int size_;
}stack;


void check(int *arr)
{
    int i = 0;
    for (i = 0; i < LEN - 1; ++i)
    {
        if (arr[i] > arr[i + 1])
        {
            printf("quick sort error\n");
            return ;
        }
    }
}

void initStack(stack *s)
{
    int *data = (int *)malloc(LEN * sizeof(int));
    if (NULL == data)
    {
        assert(0);
        return;
    }
    s->data_ = data;
    s->size_ = 0;
}

void pushStack(stack *s, int d)
{
    if (s->size_ > LEN)
    {
        printf("error size > LEN\n");
        return;
    }
    else
    {
        s->data_[s->size_++] = d;
    }
}

void popStack(stack *s)
{
    if (s->size_ > 0)
    {
        s->data_[s->size_--] = 0;
        return ;
    }

    printf("pop: error stack empty\n");
    return;
}

int topStack(stack *s)
{
    if (s->size_ > 0)
    {
        return s->data_[s->size_ - 1];
    }

    printf("top: error stack empry\n");
    return 0;
}

int emptyStack(stack *s)
{
    return s->size_ == 0;
}

int quickSort(int *arr, int begin, int end)
{
    int key = arr[end];
    int k = end;

    /* while (begin != end) */
    while (begin < end)
    {
        while (begin < end && arr[begin] <= key)
        {
            begin++;
        }
        arr[end] = arr[begin];
        while (begin < end && arr[end] >= key)
        {
            end--;
        }
        arr[begin] = arr[end];
    }
    arr[end] = key;
    return begin;

}

void sortQuickNor(int *arr, int size)
{
    stack s;
    int pos;
    int left = 0;
    int right = 0;
    initStack(&s);
    pushStack(&s, 0);
    pushStack(&s, size);

    while (!emptyStack(&s))
    {
        right = topStack(&s);
        popStack(&s);
        left = topStack(&s);
        popStack(&s);
        pos = quickSort(arr, left, right - 1);

        if (right > pos + 1) 
        {
            pushStack(&s, pos + 1);
            pushStack(&s, right);
        }
        if (pos > left)
        {
            pushStack(&s, left);
            pushStack(&s, pos);
        }
    }
}

int main(void)
{
    int arr[LEN] = {0};
    /* int arr[LEN] = { 51, 59, 23, 75, 30 }; */

    int i, j;
    int count = 0;
    srand(time(NULL));
    do
    {
        if (count++ > 15)
            return 0;
        for (i = 0; i < LEN; ++i)
        {
            arr[i] = rand() % 100;
        }

        struct timeval v1;
        gettimeofday(&v1, NULL);
        sortQuickNor(arr, LEN);
        struct timeval v2;
        gettimeofday(&v2, NULL);
        printf("use time: %ld\n", calTime(v1, v2));

        check(arr);
    } while (1);

    return 0;
}

