 //***************************************************************
 // @file:    redBlackTree.h
 // @author:  dingfang
 // @date    2018-11-13 13:57:05
 //***************************************************************
 


#define RED		0		//红色节点
#define BLACK	1		//黑色节点

typedef int Type;

typedef struct TreeNode
{
	unsigned char	color_;		//颜色 RED 或者 BLACK
	Type			key_;		//数据
	struct TreeNode *l_;		//左孩子
	struct TreeNode *r_;		//右孩子
	struct TreeNode *parent_;	//父节点
}Node;

typedef struct RedBlackRoot
{
	Node *node_;
}NodeRoot;

/*
 * 函数说明: 前序遍历树
 * */
void preorder(Node *tree);

/*
 * 函数说明: 中序遍历树
 * */
void inorderTree(Node *tree);

/*
 * 函数说明: 后序遍历树
 * */
void postorderTree(Node *tree);

/*
 * 函数说明: 用key寻找节点
 * */
Node *searchNode(Node *x, Type key);

/*
 * 函数说明: 创建节点
 * */
Node *createNode(Type key, Node *parent, Node *left, Node *right);

/*
 * 函数说明: 判断是否是红色节点
 * */
int isRedNode(const Node *node);

/*
 * 函数说明: 设置成黑色节点
 * */
int setBlack(Node *node);

/*
 * 函数说明: 设置成红色节点
 * */
int setRed(Node *node);

/*
 * 函数说明: 左旋转
 * */
int leftRotate(NodeRoot *root, Node *x);

/*
 * 函数说明: 右旋转
 * */
int rightRotate(NodeRoot *root, Node *x);

/*
 * 函数说明: 插入节点
 * */
int insertTree(NodeRoot *root, Node *node);

/* 
 * 函数说明: 修正插入树
 * */
int insertBalanceTree(NodeRoot *root, Node *node);

/* 
 * 函数说明: 修正删除树
 * */
int deleteBalanceTree(NodeRoot *root, Node *node);

/* 
 * 函数说明: 删除节点
 * */
int deleteTreeNode(NodeRoot *root, Node *node);

/* 
 * 函数说明: 删除键值为key节点
 * */
void deleteNode(NodeRoot *root, Type key);

/*
 * 函数说明: 销毁红黑树
 * */
void destroyRBTree(Node *root);
