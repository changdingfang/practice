 //***************************************************************
 // @file:    radixLSD.c
 // @author:  dingfang
 // @date    2018-11-15 18:53:31
 //***************************************************************
 
/* 基数排序 */
 
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

typedef struct Array
{
	int list_[LEN];
	int size_;
}ARRAY;

void radixSort(ARRAY *array1, int k, ARRAY *array2)
{
	int i;
	int j;
	int idx;
	for (i = 0; i < TEN; ++i)
	{
		//printf("i : %d\n", i);
		for (j = 0; j < array2[i].size_; ++j)
		{
			//printf("j : %d, size: %d\n", j, array2[i].size_);
			idx = (array2[i].list_[j] / k) % 10;
			array1[idx].list_[array1[idx].size_++] = array2[i].list_[j];
			array2[i].list_[j] = 0;
		}
		array2[i].size_ = 0;
	}
}

int getMax(int *arr, int len, ARRAY *array)
{
	int i, max = 0, *idx;
	for (i = 0; i < len; ++i)
	{
		idx = &array[arr[i] % 10].size_;
		array[arr[i] % 10].list_[(*idx)++] = arr[i];
		if (max < arr[i])
		{
			max = arr[i];
		}
	}
	return max;
}

void radixLSD(int *arr, int len)
{
	struct Array array[2][TEN];
	int i, k, j, max;
	int count = 0;

	for (i = 0; i < LEN; ++i)
	{
		array[0][i].size_ = 0;
		array[1][i].size_ = 0;
	}

	max = getMax(arr, len, array[0]);
	for (count = 0; max > 10; ++count)
	{
		max /= 10;
	}

	k = 1;
	printf("count: %d\n", count);
	for (i = 1; i <= max; ++i)
	{
		k *= 10;
		radixSort(array[i % 2], k, array[1 - i % 2]);
	}

	count = 0;
	k = 1 - i % 2;
	for (i = 0; i < TEN; ++i)
	{
		for (j = 0; j < array[k][i].size_; ++j)
		{
			arr[count++] = array[k][i].list_[j];
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

