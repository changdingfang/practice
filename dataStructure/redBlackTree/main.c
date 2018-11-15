 //***************************************************************
 // @file:    main.c
 // @author:  dingfang
 // @date    2018-11-14 10:12:40
 //***************************************************************
 
#include "redBlackTree.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
	int i;
	Type key;
	srand(time(NULL));
	NodeRoot *root = (NodeRoot *)malloc(sizeof(NodeRoot));
	if (NULL == root)
	{
		printf("root is null\n");
		return -1;
	}
	root->node_ = NULL;

	root->node_ = createNode(1, NULL, NULL, NULL);
	int count = 0;
	for (i = 0; i < 20; ++i)
	{
		key = rand() % 100;
# if 1
		if (searchNode(root->node_, key) != NULL)
		{//节点存在, 重新生成
			--i;
			count++;
			continue;
		}
# endif
		Node *node = createNode(key, NULL, NULL, NULL);
		insertTree(root, node);
	}
	printf("count: %d\n\n", count);
	preorder(root->node_);
	printf("\n");
	inorderTree(root->node_);
	printf("\n");
	return 0;
}
