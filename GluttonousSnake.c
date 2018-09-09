#include"GluttonousSnake.h"
#include<windows.h>
#include<stdio.h>
void cursor(int x,int y)
{
	//获得句柄
	HANDLE handle = GetStdHandle(STD_ERROR_HANDLE);
	COORD pos;
	pos.X= x;
	pos.Y= y;
	SetConsoleCursorPosition(handle, pos);
}
//地图：
void map()
{
	int i = 0;
	for ( i = 0; i <=55 ; i+=2)
	{
		//第0行
		cursor(i, 0);
		printf("□");
	}
	for (i = 0; i <=55; i += 2)
	{
		//第26行
		cursor(i, 26);
		printf("□");
	}
	for (i = 1; i <26; i ++)
	{
		//第0列
		cursor(0,i );
		printf("□");
	}
	for (i = 1; i <=26; i++)
	{
		//第54列
		cursor(54, i);
		printf("□");
	}
	printf("\n");
	return ;
}
//初始化蛇
void creat_snack(PSnack psn)
{
	PNode cur =(PNode) malloc(sizeof(Node));
	cur->x = line;
	cur->y = row;
	cur->next = NULL;
//psn->psnack_node = (PNode)malloc(sizeof(Node));
	/*cursor(pnode->x, pnode->y);
	printf(FOOD);
	PNode cur = pnode;*/
	//头插
	for (int i = 1; i < 5; i++)
	{
		psn->psnack_node = (PNode)malloc(sizeof(Node));	
        psn->psnack_node->next= cur;//头插
		cur = psn->psnack_node;
		cur->x = line ;
		cur->y = row +i;
	}
	while (cur)
	{
		cursor(cur->x, cur->y);
		printf(FOOD);
			cur = cur->next;
	}
	psn->status = ok;//蛇的状态
	psn->sleep_time = 500;//蛇的速度
	psn->Dir = left;
	psn->grade = 0;
	psn->class = 1;
	printf("\n");
}
void creat_food(PSnack psn)
{
	PNode Pfood = malloc(sizeof(Node));
	again:
	do
	{
		(Pfood->x) = rand() % 53 + 2;
	} while ((Pfood->x) % 2 != 0);////和whlile的区别？？？
	Pfood->y = rand() % 25 + 1;
	PNode cur = psn->psnack_node;//蛇头
		while ( cur)
		{
			
			if (Pfood->x == cur->x && cur->y == Pfood->y)
				goto again;
			cur = cur->next;
		}
	cursor(Pfood->x, Pfood->y);
			printf(FOOD);
			psn->Pfood = Pfood;
}
//判断是否有食物
//参数：蛇头，下一个节点
int has_food(PSnack psn,PNode newnode)
{
	return (newnode->x == psn->Pfood->x&&newnode->y == psn->Pfood->y);
	return 0;
}
//吃食物
//参数：蛇头，下一个节点
void eat_food(PSnack psn, PNode newnode)
{
	++psn->grade;
	//cursor(13,90);
	//printf("成绩：%d", psn->grade);
	PNode cur = psn->psnack_node;//蛇头
	newnode->next = cur;
	psn->psnack_node = newnode;
	cur = newnode;
	while (cur)
	{
		cursor(cur->x, cur->y);
		printf(FOOD);
		cur = cur->next;
	}
	creat_food(psn);
}
//没有食物
void no_food(PSnack psn, PNode newnode)
{
	PNode cur = psn->psnack_node;//psn：蛇
	newnode->next = cur;
	psn->psnack_node = newnode;
	cur = newnode;
	while (cur->next->next)
	{
		cursor(cur->x, cur->y);
		printf(FOOD);
		cur = cur->next;
	}
	//清空
	cursor(cur->next->x, cur->next->y);
	printf("  ");
	free(cur->next);
	cur->next = NULL;
}

void move_snack(PSnack psn)
{
	PNode newnode = malloc(sizeof(Node));
	newnode->x = psn->psnack_node->x;
	newnode->y = psn->psnack_node->y;
	switch (psn ->Dir)
	{
	case up:
		newnode->y -= 1;
		break;
	case down:
		newnode->y +=1;
		break;
	case right:
		newnode->x +=2;
		break;
	case left:
		newnode->x-=2;
		break;
	default:
		break;
	}
	if (has_food(psn, newnode))
	{
		eat_food(psn, newnode);
	}
	else
	{
		no_food(psn, newnode);
	}
}

void KillByWall(PSnack psn)
{
	if (psn->psnack_node->x == 0 || psn->psnack_node->x == 56 ||
		psn->psnack_node->y == 0 || psn->psnack_node->y == 26)
		psn->status = kill_by_wall;
}
void KillBySelf(PSnack psn)
{
	PNode cur = psn->psnack_node->next;
	while (cur != NULL)
	{
		if (cur->x == psn->psnack_node->x&&cur->y == psn->psnack_node->y)
		{
			psn->status = kill_by_self;
				break;
		}
		cur = cur->next;
	}
}


