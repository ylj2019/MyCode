# other
#define _CRT_SECURE_NO_WARNINGS 1

#include<stdio.h>
#include<malloc.h>
#include<cassert>
#include"Stack .h"
#include"bin_tree.h"
#include"queue.h"
//typedef struct BinTreeNode
//{
//	struct BinTreeNode* _pLeft;
//	struct BinTreeNode* _pRight;
//	BTDataType _data;
//}BTNode, *PBTNode;

PBTNode BuyBinTreeNode(BTDataType data)
{
	PBTNode pnew_node = (PBTNode)malloc(sizeof(BTNode));
	assert(pnew_node);
	pnew_node->_data = data;
	pnew_node->_pLeft = NULL;
	pnew_node->_pRight = NULL;
	return pnew_node;
}
void CreateBinTree(PBTNode* pRoot, BTDataType* array, int size, BTDataType invalid)
{
	int index=0;
   _CreateBinTree(pRoot, array,  size,&index,  invalid);

}
//想象
void _CreateBinTree(PBTNode* pRoot, BTDataType* array, int size, int* index, BTDataType invalid)
{
	if (*index < size&&array[*index] != invalid)
	{
		*pRoot = BuyBinTreeNode(array[*index]);
		(*index)++;
		_CreateBinTree(&(*pRoot)->_pLeft, array, size, index, invalid);
		(*index)++;
		_CreateBinTree(&(*pRoot)->_pRight, array, size, index, invalid);
	}

}


// 拷贝 
PBTNode CopyBinTree(PBTNode pRoot)
{
	PBTNode pNewRoot = NULL;
	if (pRoot)
	{
		// 拷贝当前树的根节点
		pNewRoot = BuyBinTreeNode(pRoot->_data);

		// 拷贝当前树的左子树
		if (pRoot->_pLeft)
			pNewRoot->_pLeft = CopyBinTree(pRoot->_pLeft);

		// 拷贝当前树的右子树
		if (pRoot->_pRight)
			pNewRoot->_pRight = CopyBinTree(pRoot->_pRight);
	}
	
	return pNewRoot;
}

// 前序递归遍历 
void PreOrder(PBTNode pRoot)
{
	if (pRoot)
	{
		printf("%c", pRoot->_data);
		PreOrder(pRoot->_pLeft);
		PreOrder(pRoot->_pRight);
	}

}

// 前序非递归 
void PreOrderNor(PBTNode pRoot)
{
	PBTNode cur = pRoot;
	static s;
	StackInit(&s);
	if (pRoot == NULL)
		return;
	while (cur || StackEmpty(&s))
	{
		while (cur)
		{
			StackPush(&s, cur);
			printf("%c", cur->_data);
			cur = cur->_pLeft;
		}
		cur = StackTop(&s);
		//printf("%c", cur->_data);
		StackPop(&s);
		cur = cur->_pRight;
	}
}
// 前序非递归 
void PreOrderNor1(PBTNode pRoot)
{
	PBTNode cur = pRoot;
	Stack s;
	StackInit(&s);
	if (pRoot == NULL)
		return 0;
	
	while (cur||StackEmpty(&s))
	{
		while (cur)
		{
			StackPush(&s, cur);
			printf("%c", cur->_data);
			cur = cur->_pLeft;
		}
		cur = StackTop(&s);
		StackPop(&s);
		cur = cur->_pRight;
	}
}
// 中序递归 
void InOrder(PBTNode pRoot)
{
	if (pRoot)
	{
		InOrder(pRoot->_pLeft);
		printf("%c", pRoot->_data);
		InOrder(pRoot->_pRight);
	}
}

// 中序非递归 
/*
找以cur为根的树最左侧的节点，并保存所经路径中所有节点
*/
void InOrderNor(PBTNode pRoot)
{
	PBTNode cur = pRoot;
	PBTNode pre = pRoot;
	PBTNode top = pRoot;
	Stack s;
	StackInit(&s);
	if (pRoot == NULL)
		return 0;
	
	while (cur || StackEmpty(&s))
	{
		while (cur)
		{
			StackPush(&s, cur);
			cur = cur->_pLeft;
		}
		cur = StackTop(&s);
		printf("%c", cur->_data);
		StackPop(&s);

		cur = cur->_pRight;
	}
}

// 后续递归 
void PostOrder(PBTNode pRoot)
{
	if (pRoot)
	{
		PostOrder(pRoot->_pLeft);
		PostOrder(pRoot->_pRight);
		printf("%c", pRoot->_data);
	}

}

// 后序非递归 
void PostOrder1(PBTNode pRoot)
{
	PBTNode cur = pRoot;
	PBTNode pre = pRoot;
	PBTNode top = pRoot;
	Stack s;
	StackInit(&s);
	if (pRoot == NULL)
		return 0;
	
	while (cur || StackEmpty(&s))
	{
		while (cur)
		{
			StackPush(&s, cur);
			cur = cur->_pLeft;
		}
		top = StackTop(&s);
		if (top->_pRight == NULL || top->_pRight == pre)
		{
			pre = top;
			printf("%c", top->_data);
			StackPop(&s);
		}
		else
			cur = cur->_pRight;
	}
}

// 求二叉树中结点的个数 
int Size(PBTNode pRoot)
{
	int num = 0;
	if (pRoot == NULL)
		return 0;
	else
	{
		num = 1 + Size(pRoot->_pLeft) + Size(pRoot->_pRight);
	}
	return num;
}

// 求二叉树中叶子结点的个数 
int GetLeafCount(PBTNode pRoot)
{
	
		if (NULL == pRoot)
		return 0;

		if (NULL == pRoot->_pLeft && NULL == pRoot->_pRight)
			return 1;

		return GetLeafCount(pRoot->_pLeft) + GetLeafCount(pRoot->_pRight);


}

// 获取第K层中结点个数 
int GetKLevel(PBTNode pRoot, int K)
{
	if (pRoot == NULL || K < 1)
		return 0;
	if (1 == K)
		return 1;
	return GetKLevel(pRoot->_pLeft, K - 1) + GetKLevel(pRoot->_pRight, K - 1);
}

// 获取二叉树的高度 
int Height(PBTNode pRoot)
{
	int left_height, right_height;
	if (pRoot == NULL)
		return 0;
	left_height = Height(pRoot->_pLeft);
	right_height = Height(pRoot->_pRight);
	return left_height > right_height ? left_height + 1 : right_height + 1;
}

// 二叉树层序遍历 
void LevelOrder(PBTNode pRoot)
{
	//1.取队头元素
	//2.访问
	//3.左子树入队列，右子树入列
	//4.出队列
	queue q;
	if (pRoot == NULL)
		return;
	
	queue_init(&q);
	queue_push(&q, pRoot);
	PBTNode cur = pRoot;
	PBTNode tmp = pRoot;
	while (queue_empty(&q)!=0)
	{
		cur = queue_front(&q);
		tmp = queue_front(&q);
		printf("%c",tmp->_data);
		if (cur->_pLeft)
		queue_push(&q, cur->_pLeft);
		if (cur->_pRight)
		queue_push(&q, cur->_pRight);
		queue_pop(&q);
		
	}
	
}

void swap(PBTNode*left, PBTNode*right)
{
	PBTNode tmp = NULL;
	tmp = *left;
	*left = *right;
	*right = tmp;
}
// 二叉树镜像递归 
void Mirror(PBTNode pRoot)
{
	if (pRoot)
	{
		swap(pRoot->_pLeft, pRoot->_pRight);
		Mirror(pRoot->_pLeft);
		Mirror(pRoot->_pRight);
	}
}

// 二叉树镜像非递归 
void Mirror_Nor(PBTNode pRoot)
{
	queue q;
	if (pRoot)
		return;
	
	queue_init(&q);
	if (pRoot)
		queue_push(&q, pRoot);
	while (queue_empty!=0)
	{
		PBTNode cur = queue_front(&q);
		swap(cur->_pLeft, cur->_pRight);
		if (cur->_pLeft)
		queue_push(&q, cur->_pLeft);
		if (cur->_pRight)
		queue_push(&q, cur->_pRight);
		queue_pop(&q);
	}
}

// 判断一棵树是否是完全二叉树
/*
从上往下找，找到第一个没有孩子或只有左孩子，
它后面的节点就不能有孩子节点
*/
int IsCompleteBinTree(PBTNode pRoot)
{
	int flag = 0;
	queue q;
	queue_init(&q);
	PBTNode cur = NULL;
	if (NULL == pRoot)
		return 1;
	queue_push(&q, pRoot);
	while (queue_empty(&q)!=0)
	{
		cur = queue_front(&q);
		queue_pop(&q);
		if (flag == 1)
		{
			if (cur->_pLeft || cur->_pRight)
				return 0;
		}
		else
		{
			if (cur->_pLeft&&cur->_pRight)//左右都存在
			{
				queue_push(&q, cur->_pLeft);
				queue_push(&q, cur->_pRight);
			}
			else if (cur->_pLeft)//只有左
			{
				flag = 1;
				queue_push(&q, cur->_pLeft);
			}
			else if (cur->_pRight)//只有右
				return 0;
			else//左右都没有
				flag = 1;
		}
		
	}
	return 1;

}

// 在二叉树中查找值为data的结点 
PBTNode Find(PBTNode pRoot, BTDataType data)
{
	PBTNode pRet = NULL;
	if (pRoot == NULL)
		return NULL;
	if (data == pRoot->_data)
		return pRoot;
	if (pRet = Find(pRoot->_data, data))
		return pRet;
	return Find(pRoot->_pLeft, data);
}

// 判断一个节点是否在二叉树中 
int IsNodeInBinTree(PBTNode pRoot, PBTNode pNode)
{
	int ret = 0;
	if (NULL == pRoot || NULL == pNode)
		return 0;
	if (pRoot == pNode)
		return 1;
	if (ret = IsNodeInBinTree(pRoot->_pLeft, pNode))
		return ret;
	return  IsNodeInBinTree(pRoot->_pRight, pNode);
}

// 获取当前节点的左孩子 
PBTNode LeftChild(PBTNode pNode);

// 获取当前节点的右孩子 
PBTNode RightChild(PBTNode pNode);
//销毁
void DestroyBinTree(PBTNode* pRoot)
{
	assert(pRoot);
	if (*pRoot)
	{
		DestroyBinTree(&(*pRoot)->_pLeft);
		DestroyBinTree(&(*pRoot)->_pRight);
		free(*pRoot);
		*pRoot = NULL;
	}
}
