 //***************************************************************
 // @file:    heap.c
 // @author:  dingfang
 // @date    2018-11-15 17:29:31
 //***************************************************************
 
/* 堆排序 */
 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define LEN 8


void print(int *arr)
{
	int i = 0;
	for (i = 0; i < LEN; ++i)
	{
		printf("%3d", arr[i]);
	}
	printf("\n");
}

void swap(int *x, int *y)
{
	int k;
	k = *x;
	*x = *y;
	*y = k;
}

void check(int *arr)
{
	int i = 0;
	for (i = 0; i < LEN - 1; ++i)
	{
		if (arr[i] > arr[i + 1])
		{
			print(arr);
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
			swap(&a[dad], &a[son]);
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
		swap(&arr[0], &arr[i]);
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
			arr[i] = rand() % 100;
		}

		print(arr);

		heapSort(arr, LEN);
		print(arr);
		printf("\n");

		//check(arr);
	} while (0);
	//print(arr);

	return 0;
}

