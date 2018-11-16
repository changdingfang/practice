 //***************************************************************
 // @file:    2radixLSD.c
 // @author:  dingfang
 // @date    2018-11-16 11:53:31
 //***************************************************************
 
/* 基数排序2 */
 
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

void radixSort(int *arr1, int *arr2, int k, int len)
{
	int i;
	int buckets[TEN] = {0};

	for (i = 0; i < len; ++i)
	{
		buckets[(arr2[i] / k) % 10]++;
	}

	for (i = 1; i < TEN; ++i)
	{
		buckets[i] += buckets[i - 1];
	}

	for (i = len - 1; i >= 0; --i)
	{
		arr1[buckets[(arr2[i] / k) % 10] - 1] = arr2[i];
		buckets[(arr2[i] /k) % 10]--;
	}
}

int getMax(int *arr, int len)
{
	int i, max = 0;
	for (i = 0; i < len; ++i)
	{
		if (max < arr[i])
		{
			max = arr[i];
		}
	}
	return max;
}

void radixLSD(int *arr, int len)
{
	int i, k, j, max;
	int count = 0;
	int arr1[LEN] = {0};

	for (i = 0; i < LEN; ++i)
	{
	}

	max = getMax(arr, len);
	for (count = 0; max > 10; ++count)
	{
		max /= 10;
	}

	k = 1;
	printf("count: %d\n", count);
	for (i = 0; i < max; ++i)
	{
		if (i % 2 == 0)
		{
			radixSort(arr1, arr, k, len);
		}
		else
		{
			radixSort(arr, arr1, k, len);
		}
		k *= 10;
	}

	if (i % 2 == 1)
	{
		for (i = 0; i < TEN; ++i)
		{
			arr[i] = arr1[i];
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
		if (count++ > 5)
			break;
		for (i = 0; i < LEN; ++i)
		{
			arr[i] = rand() % 10000;
		}

		print(arr);

		radixLSD(arr, LEN);
		print(arr);
		printf("\n");

		//check(arr);
	} while (0);
	//print(arr);

	return 0;
}

