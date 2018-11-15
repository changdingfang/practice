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

#define LEN 12

typedef int Type;

typedef struct Stack
{
	int *data_;
	int size_;
}stack;

void print(int *arr)
{
	int i = 0;
	for (i = 0; i < LEN; ++i)
	{
		printf("%3d", arr[i]);
	}
	printf("\n");
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

void initStack(stack *s)
{
	Type *data = (int *)malloc(LEN * sizeof(Type));
	if (NULL == data)
	{
		assert(0);
		return;
	}
	s->data_ = data;
	s->size_ = 0;
}

void pushStack(stack *s, Type d)
{
	assert(s);
	if (s->size_ > LEN)
	{
		return;
	}
	else
	{
		s->data_[s->size_++] = d;
	}
}

void popStack(stack *s)
{
	assert(s);
	if (s->size_ > 0)
	{
		s->data_[s->size_--] = 0;
	}
	return;
}

Type topStack(stack *s)
{
	assert(s);
	if (s->size_ > 0)
	{
		return s->data_[s->size_ - 1];
	}
	return 0;
}

int emptyStack(stack *s)
{
	assert(s);
	return s->size_ == 0;
}

int quickSort(int *arr, int begin, int end)
{
	Type key = arr[end];
	int k = end;

	while (begin != end)
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

		if ((right - left) > pos + 1)
		{
			pushStack(&s, pos + 1);
			pushStack(&s, right - left);
		}
		if (pos > 0)
		{
			pushStack(&s, 0);
			pushStack(&s, pos);
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
			return 0;
		for (i = 0; i < LEN; ++i)
		{
			arr[i] = rand() % 100;
		}

		print(arr);

		sortQuickNor(arr, LEN);
		print(arr);
		printf("\n");

		//check(arr);
	} while (1);
	print(arr);

	return 0;
}

