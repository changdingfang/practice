 //***************************************************************
 // @file:    merge.c
 // @author:  dingfang
 // @date    2018-11-16 14:53:31
 //***************************************************************
 
/* 归并排序 */
 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define LEN 10
#define TEN 10

void print(int *arr)
{
	int i = 0;
	for (i = 0; i < LEN; ++i)
	{
		printf("%6d", arr[i]);
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

int merge(int * a, int *b, int left,int mid, int right)
{
	int l = left;
	int r = right;
	int m = mid + 1;
	int k = 0;
	while (l <= mid && m <= right)
	{
		if (a[l] > a[m])
		{
			b[k++] = a[m++];
		}
		else
		{
			b[k++] = a[l++];
		}
	}
	while (l <= mid)
	{
		b[k++] = a[l++];
	}
	while (m <= right)
	{
		b[k++] = a[m++];
	}
	int i;
	for (l = left, i = 0; i < k; ++i, ++l)
	{
		a[l] = b[i];
	}
}

int mergeSort(int *a, int *b, int left, int right)
{
	int mid = (left + right) >> 1 ;

	if (left < right)
	{
		mergeSort(a, b, left, mid);
		mergeSort(a, b, mid + 1, right);
		merge(a, b, left, mid, right);
	}
}

int main(void)
{
	int a[LEN] = {0};
	int b[LEN] = {0};

	int i, j;
	int count = 0;
	srand(time(NULL));
	do
	{
		if (count++ > 5)
			break;
		for (i = 0; i < LEN; ++i)
		{
			a[i] = rand() % 10000;
		}

		print(a);

		mergeSort(a, b, 0, LEN - 1);
		print(a);
		printf("\n");

		//check(a);
	} while (0);

	return 0;
}

