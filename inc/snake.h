#ifndef SNAKE_H__
#define SNAKE_H__

#include <stdbool.h>

#include "windows.h"

#define WALL (L'□')
#define BODY (L'●')
#define FOOD (L'★')

#define START_X (24)
#define START_Y (5)

#ifdef __cplusplus
extern "C" {
#endif  //__cplusplus

/**
 * @brief 蛇头的移动方向
 */
typedef enum Direction {
  UP = 0,  // 上
  DOWN,    // 下
  LEFT,    // 左
  RIGHT    // 右
} Direction_t;

typedef enum GameStatus {
  GAME_RUNNING = 0,  // 游戏进行中
  GAME_OVER,         // 游戏结束
  GAME_PAUSED,       // 游戏暂停
  END_NORMAL,
  KILL_BY_SELF,
  KILL_BY_WALL,
} GameStatus_t;

typedef struct SnakeNode {
  int x;
  int y;
  struct SnakeNode* next;
} SnakeNode_t;

/**
 * @struct Snake
 * @brief 蛇总体结构
 */
typedef struct Snake {
  SnakeNode_t* p_head;    // 指向贪吃蛇头结点的指针
  SnakeNode_t* p_food;    // 指向食物节点的指针
  int score;              // 贪吃蛇累积的总分
  int food_weight;        // 一个食物的分数
  int sleep_time;         // 每走一步的时间间隔，时间越短，速度越快
  Direction_t direction;  // 当前蛇的移动方向
  GameStatus_t status;    // 游戏状态
} Snake_t;

inline bool KeyIsPressed(short key) {
  return (GetAsyncKeyState(key) & 0x01) == 1 ? true : false;
}

inline bool IsFood(Snake_t* p_snake, SnakeNode_t* p_next) {
  return p_snake->p_food->x == p_next->x && p_snake->p_food->y == p_next->y;
}

void GameStart(Snake_t* snake);
void WelcomeToGame(void);
void SetPos(short x, short y);
void CreateMap(void);
void InitSnake(Snake_t* p_snake);
void CreateFood(Snake_t* p_snake);
void GameRun(Snake_t* p_snake);
void GamePause(void);
void SnakeMove(Snake_t* p_snake);
void EatFood(Snake_t* p_snake, SnakeNode_t* p_next);
void NoFood(Snake_t* p_snake, SnakeNode_t* p_next);
void KillByWall(Snake_t* p_snake);
void KillBySelf(Snake_t* p_snake);
void GameEnd(Snake_t* p_snake);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // SNAKE_H__
