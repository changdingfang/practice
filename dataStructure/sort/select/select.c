 //***************************************************************
 // @file:    select.c
 // @author:  dingfang
 // @date    2018-11-14 17:03:35
 //***************************************************************
 
/* 选择排序 */
 
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

	int i, j, max;
	max = arr[0];
	srand(time(NULL));
	for (i = 0; i < LEN; ++i)
	{
		arr[i] = rand() % 100;
	}

	print(arr);
	for (i = 0; i < LEN - 1; ++i)
	{
		max = 0;
		for (j = 1; j < LEN - i; ++j)
		{
			if (arr[max] < arr[j])
			{
				max = j;
			}
		}
		swap(&arr[max], &arr[LEN -i - 1]);
	}

	print(arr);

	return 0;
}

