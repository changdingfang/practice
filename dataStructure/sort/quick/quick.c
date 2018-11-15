 //***************************************************************
 // @file:    quick.c
 // @author:  dingfang
 // @date    2018-11-14 17:05:35
 //***************************************************************
 
/* 快排 */
 
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
			print(arr);
			break;
		}
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
		if (count++ > 10000)
			break;
		for (i = 0; i < LEN; ++i)
		{
			arr[i] = rand() % 100;
		}

		print(arr);

		sortQuick(arr, 0, LEN - 1);

		//check(arr);
	} while (0);
	print(arr);

	return 0;
}

