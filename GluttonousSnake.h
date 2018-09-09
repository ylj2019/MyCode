#ifndef __SNACK_H_
#define __SNACK_H_
 
#include<windows.h>
#include<stdio.h>
#define FOOD "□"
#define line 16
#define row 15
 typedef struct node//放"□"的坐标的变量
{
	 int x;
	 int y;
	 struct node*next;

 }Node, *PNode;
 enum  Status
 {
	 ok,
	 kill_by_self,
	 kill_by_wall,
	 quit,
 };
 enum Dir
 {
 up,
 down,
 left,
 right,
 };
 typedef struct snack
 {
	 PNode psnack_node;//指向蛇头
	 PNode Pfood;//指向食物
	 enum  status status;//蛇的状态
	 enum Dir Dir;//蛇走的方向
	 int sleep_time;//每走一步停留的时间
	 int grade;//成绩
	 int class ;//等级
 }Snack, *PSnack;
void map();
void cursor(int x, int y);
void creat_snack(PSnack psn);
void creat_food(PSnack psn);
void move_snack(PSnack psn);
void run_snack(PSnack psn);
void KillByWall(PSnack psn);
void KillBySelf(PSnack psn);

#endif;
