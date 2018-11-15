 //***************************************************************
 // @file:    insertion.c
 // @author:  dingfang
 // @date    2018-11-14 15:53:38
 //***************************************************************
 
/* 插入排序 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define LEN 6

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

int main(void)
{
	int arr[LEN] = {0};

	int i, j, max;
	srand(time(NULL));
	for (i = 0; i < LEN; ++i)
	{
		arr[i] = rand() % 100;
	}

	print(arr);

	int tmp = 0;
	for (i = 0; i < LEN - 1; ++i)
	{
		tmp = arr[i + 1];
		for (j = i; j >= 0; --j)
		{
			if (tmp < arr[j])
			{
				arr[j + 1] = arr[j];
				arr[j] = tmp;
			}
		}
	}

	print(arr);

	return 0;
}
