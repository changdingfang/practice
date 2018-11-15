 //***************************************************************
 // @file:    main.c
 // @author:  dingfang
 // @date    2018-11-14 15:29:35
 //***************************************************************
 
/* 冒泡排序 */
 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define LEN 20


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

	int i, j;
	srand(time(NULL));
	for (i = 0; i < LEN; ++i)
	{
		arr[i] = rand() % 100;
	}

	print(arr);
	int tmp;
	for (i = 0; i < LEN - 1; ++i)
	{
		for (j = LEN - 1; j > i; --j)
		{
			if (arr[j] < arr[j - 1])
			{
				swap(&arr[j], &arr[j - 1]);
			}
		}
	}

	print(arr);

	return 0;
}

