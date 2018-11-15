 //***************************************************************
 // @file:    redBlackTree.c
 // @author:  dingfang
 // @date    2018-11-13 14:05:02
 //***************************************************************
 
#include "redBlackTree.h"

#include <stdio.h>
#include <stdlib.h>

void print(Node *tree)
{
	if (BLACK == tree->color_)
	{
		printf("%4d-", tree->key_);
	}
	else
	{
		printf("%4d+", tree->key_);
	}
}

void preorder(Node *tree)
{
	if (NULL != tree)
	{
		print(tree);
		preorder(tree->l_);
		preorder(tree->r_);
	}
}

void inorderTree(Node *tree)
{
	if (NULL != tree)
	{
		inorderTree(tree->l_);
		print(tree);
		inorderTree(tree->r_);
	}
}

void postorderTree(Node *tree)
{
	if (NULL != tree)
	{
		postorderTree(tree->l_);
		postorderTree(tree->r_);
		print(tree);
	}
}

Node *searchNode(Node *x, Type key)
{
	while ((NULL != x) && (x->key_ != key))
	{
		if (x->key_ > key)
		{
			x = x->l_;
		}
		else
		{
			x = x->r_;
		}
	}
	return x;
}

Node *createNode(Type key, Node *parent, Node *left, Node *right)
{
	Node *p;
	p = (Node *)malloc(sizeof(Node));
	if (NULL == p)
	{
		return NULL;
	}
	p->key_		= key;
	p->parent_	= parent;
	p->l_		= left;
	p->r_		= right;
	p->color_	= BLACK;

	return p;
}

int isRedNode(const Node *node)
{
	return RED == node->color_;
}

int setBlack(Node *node)
{
	node->color_ = BLACK;
	return 0;
}

int setRed(Node *node)
{
	node->color_ = RED;
}

/* * * * * * * * * * * * * * * * * * * * * *
*     px                             px    *
*     /                              /     *
*    x          (左旋转)           y       *
*   / \         -------->         / \      *
*  lx  y                         x  ry     *
*     / \                       / \        *
*    ly  ry                    lx  ly      *
* * * * * * * * * * * * * * * * * * * * * */
int leftRotate(NodeRoot *root, Node *x)
{
	if (NULL == root || NULL == x)
	{
		return -1;
	}
	Node *y = x->r_;
	if (NULL != y)
	{
		y->parent_	= x->parent_;
		if (x->parent_ == NULL)
		{
			root->node_ = y;
		}
		else
		{
			if (x->parent_->l_ == x)
			{
				x->parent_->l_	= y;
			}
			else
			{
				x->parent_->r_ = y;
			}
		}
		if (NULL != y->l_)
		{
			y->l_->parent_	= x;
		}
		x->r_			= y->l_;
		x->parent_	= y;
		y->l_		= x;
	}

	return 0;
}

/* * * * * * * * * * * * * * * * * * * * * *
*     py                             py    *
*     /                              /     *
*    y			(右旋转)           x       *
*   / \			-------->         / \      *
*  x   ry                        lx  y     *
* / \                               / \    *
*lx  rx                            rx  ry  *
* * * * * * * * * * * * * * * * * * * * * */
int rightRotate(NodeRoot *root, Node *y)
{
	if (NULL == root || NULL == y)
	{
		return -1;
	}
	Node *x = y->l_;
	if (NULL != x)
	{
		if (NULL != x->r_)
		{
			x->r_->parent_ = y;
		}
		y->l_ = x->r_;
		x->r_ = y;
		
		x->parent_ = y->parent_;
		if (NULL == y->parent_)
		{
			root->node_ = x;
		}
		else
		{
			if (y->parent_->l_ == y)
			{
				y->parent_->l_ = x;
			}
			else
			{
				y->parent_->r_ = x;
			}
		}
		y->parent_ = x;
	}
}

int insertTree(NodeRoot *root, Node *node)
{
	Node *y = NULL;
	Node *x = root->node_;
	if (NULL == x)
	{
		root->node_ = node;
		node->color_ = BLACK;
		return 0;
	}

	while (NULL != x)
	{
		y = x;
		if (node->key_ > x->key_)
		{
			x = x->r_;
		}
		else
		{
			x = x->l_;
		}
	}
	node->parent_ = y;
	if (NULL != y)
	{
		if (node->key_ > y->key_)
		{
			y->r_ = node;
		}
		else
		{
			y->l_ = node;
		}
	}
	node->color_ = RED;
	insertBalanceTree(root, node);
}

int insertBalanceTree(NodeRoot *root, Node *node)
{
	Node *parent = NULL, *gparent = NULL;

	while ((parent = node->parent_) && isRedNode(parent))
	{
		//父节点为红色,那么父节点不是根节点, 所以祖父节点肯定不会为空
		gparent = parent->parent_;

		if (parent == gparent->l_)
		{//父节点是祖父节点的左孩子
			//1.叔叔是红色节点
			{
				Node *uncle = gparent->r_;
				if (uncle && isRedNode(uncle))
				{
					setBlack(uncle);
					setBlack(parent);
					setRed(gparent);
					node = gparent;
					continue;
				}
			}
			//2.叔叔节点是黑色, 且当前节点是右孩子
			if (parent->r_ == node)
			{
				Node *tmp;
				leftRotate(root, parent);
				tmp = parent;
				parent = node;
				node = tmp;
			}
			//3.叔叔节点是黑色, 且当前节点是左孩子
			setBlack(parent);
			setRed(gparent);
			rightRotate(root, gparent);
		}
		else //父节点是祖父节点的右孩子
		{
			//1. 叔叔节点是红色
			{
				Node *uncle = gparent->l_;
				if (uncle && isRedNode(uncle))
				{
					setBlack(uncle);
					setBlack(parent);
					setRed(gparent);
					node = gparent;
					continue;
				}
			}
			//2. 叔叔节点是黑色, 且当前节点是左孩子
			if (parent->l_ == node)
			{
				Node *tmp;
				rightRotate(root, parent);
				tmp = parent;
				parent = node;
				node = tmp;
			}
			//3. 叔叔节点是黑色, 且当前节点是右孩子
			setBlack(parent);
			setRed(gparent);
			leftRotate(root, gparent);
		}
	}
	setBlack(root->node_);
}

int deleteBalanceTree(NodeRoot *root, Node *node)
{//留空
	return 0;
}

int deleteTreeNode(NodeRoot *root, Node *node)
{//留空
	return 0;
}

void deleteNode(NodeRoot *root, Type key)
{
	Node *node;

	if (NULL != (node = searchNode(root->node_, key)))
	{
		deleteTreeNode(root, node);
	}
}

void destroyRBTree(Node *root)
{
	if (NULL == root)
	{
		return ;
	}
	if (NULL != root->l_)
	{
		destroyRBTree(root->l_);
	}
	if (NULL != root->r_)
	{
		destroyRBTree(root->r_);
	}
	free(root);
}
