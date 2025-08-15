#ifndef SNAKE_H__
#define SNAKE_H__

#include <stdbool.h>

#include "windows.h"

#define WALL (L'��')
#define BODY (L'��')
#define FOOD (L'��')

#define START_X (24)
#define START_Y (5)

#ifdef __cplusplus
extern "C" {
#endif  //__cplusplus

/**
 * @brief ��ͷ���ƶ�����
 */
typedef enum Direction {
  UP = 0,  // ��
  DOWN,    // ��
  LEFT,    // ��
  RIGHT    // ��
} Direction_t;

typedef enum GameStatus {
  GAME_RUNNING = 0,  // ��Ϸ������
  GAME_OVER,         // ��Ϸ����
  GAME_PAUSED,       // ��Ϸ��ͣ
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
 * @brief ������ṹ
 */
typedef struct Snake {
  SnakeNode_t* p_head;    // ָ��̰����ͷ����ָ��
  SnakeNode_t* p_food;    // ָ��ʳ��ڵ��ָ��
  int score;              // ̰�����ۻ����ܷ�
  int food_weight;        // һ��ʳ��ķ���
  int sleep_time;         // ÿ��һ����ʱ������ʱ��Խ�̣��ٶ�Խ��
  Direction_t direction;  // ��ǰ�ߵ��ƶ�����
  GameStatus_t status;    // ��Ϸ״̬
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
