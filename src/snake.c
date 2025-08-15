#include "snake.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "windows.h"

#define _CRT_SECURE_NO_WARNINGS

/**
 * @brief 开始游戏
 * @param p_snake
 */
void GameStart(Snake_t* p_snake) {
  // 控制台窗口的设置
  system("mode con cols=100 lines=30");
  system("title 贪吃蛇");

  // 隐藏光标
  HANDLE console_output = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_CURSOR_INFO cursor_info;
  GetConsoleCursorInfo(console_output, &cursor_info);
  cursor_info.bVisible = false;
  SetConsoleCursorInfo(console_output, &cursor_info);

  // 打印欢迎界面
  WelcomeToGame();

  // 创建地图
  CreateMap();

  // 初始化贪吃蛇
  InitSnake(p_snake);
  // getchar();

  // 创建食物
  CreateFood(p_snake);
}

void WelcomeToGame(void) {
  SetPos(40, 14);
  printf("欢迎来到贪吃蛇小游戏\n");
  SetPos(40, 25);
  system("pause");
  system("cls");
  SetPos(24, 15);
  printf("使用 ↑ ↓ ← → 分别控制蛇的移动，F3 是加速，F4 是减速");
  SetPos(40, 25);
  system("pause");
  system("cls");
}

/**
 * @brief 设置光标的位置
 * @param x 横坐标
 * @param y 纵坐标
 */
void SetPos(short x, short y) {
  COORD position;
  position.X = x;
  position.Y = y;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
}

void CreateMap(void) {
  // 上
  SetPos(0, 0);

  for (int i = 0; i <= 56; i += 2) {
    wprintf(L"%lc", WALL);
  }

  // 下
  SetPos(0, 26);

  for (int i = 0; i <= 56; i += 2) {
    wprintf(L"%lc", WALL);
  }

  // 左
  for (int i = 0; i <= 25; i++) {
    SetPos(0, i);
    wprintf(L"%lc", WALL);
  }

  // 右
  for (int i = 0; i <= 25; i++) {
    SetPos(56, i);
    wprintf(L"%lc", WALL);
  }

  // getchar();
}

/**
 * @brief 初始化蛇身
 * @param p_snake
 */
void InitSnake(Snake_t* p_snake) {
  for (int i = 0; i < 5; i++) {
    SnakeNode_t* p_current = (SnakeNode_t*)malloc(sizeof(SnakeNode_t));

    if (NULL == p_current) {
      perror("InitSnake()::malloc()");
      return;
    }

    p_current->x = START_X + 2 * i;
    p_current->y = START_Y;
    p_current->next = NULL;

    if (NULL == p_snake->p_head) {
      p_snake->p_head = p_current;
    } else {
      p_current->next = p_snake->p_head;
      p_snake->p_head = p_current;
    }
  }

  // 打印蛇身
  SnakeNode_t* p_current = p_snake->p_head;

  while (NULL != p_current) {
    SetPos(p_current->x, p_current->y);
    wprintf(L"%lc", BODY);
    p_current = p_current->next;
  }

  p_snake->status = GAME_RUNNING;
  p_snake->score = 0;
  p_snake->p_food = 0;
  p_snake->sleep_time = 200;
  p_snake->food_weight = 10;
  p_snake->direction = RIGHT;
}

/**
 * @brief 创建食物
 * @param p_snake
 * @note  1. 坐标应该是随机生成的
 *        2. 食物的坐标不能在墙外
 *        3. 食物的 x 坐标必须是 2 的倍数
 *  2 <= x <= 54, 1 <= y <= 25
 */
void CreateFood(Snake_t* p_snake) {
  COORD position;

  // 保证 x 为偶数
again:
  do {
    position.Y = rand() % 25 + 1;
    position.X = rand() % 53 + 2;
  } while (position.X % 2 != 0);

  // 坐标不能和蛇的身体冲突
  SnakeNode_t* p_current = p_snake->p_head;

  while (p_current != NULL) {
    // 比较坐标
    if (position.X == p_current->x && position.X == p_current->y) {
      goto again;
    }

    p_current = p_current->next;
  }

  SnakeNode_t* p_food = (SnakeNode_t*)malloc(sizeof(SnakeNode_t));

  if (NULL == p_food) {
    perror("CreateFood()::malloc()");
    return;
  }

  p_food->x = position.X;
  p_food->y = position.Y;

  p_snake->p_food = p_food;

  // 打印食物
  SetPos(p_food->x, p_food->y);
  wprintf(L"%lc", FOOD);
}

void PrintHelpInfo(void) {
  SetPos(60, 14);
  printf("1. 不能穿墙，不能咬到自己");
  SetPos(60, 15);
  printf("2. 使用 ↑ ↓ ← → 分别控制蛇的移动");
  SetPos(60, 16);
  printf("3. F3 加速，F4 减速");
  SetPos(60, 17);
  printf("4. ESC 退出, 空格暂停游戏");
}

void GameRun(Snake_t* p_snake) {
  PrintHelpInfo();

  do {
    SetPos(60, 9);
    printf("得分：%05d", p_snake->score);
    SetPos(60, 10);
    printf("每个食物的分数：%2d", p_snake->food_weight);

    if (KeyIsPressed(VK_UP) && p_snake->direction != DOWN) {
      p_snake->direction = UP;
    } else if (KeyIsPressed(VK_DOWN) && p_snake->direction != UP) {
      p_snake->direction = DOWN;
    } else if (KeyIsPressed(VK_LEFT) && p_snake->direction != RIGHT) {
      p_snake->direction = LEFT;
    } else if (KeyIsPressed(VK_RIGHT) && p_snake->direction != LEFT) {
      p_snake->direction = RIGHT;
    } else if (KeyIsPressed(VK_F3)) {  // 加速
      if (p_snake->sleep_time >= 80) {
        p_snake->sleep_time -= 30;
        p_snake->food_weight += 2;
      }
    } else if (KeyIsPressed(VK_F4)) {  // 减速
      if (p_snake->food_weight >= 2) {
        p_snake->sleep_time += 30;
        p_snake->food_weight -= 2;
      }
    } else if (KeyIsPressed(VK_SPACE)) {
      GamePause();
    } else if (KeyIsPressed(VK_ESCAPE)) {
      p_snake->status = GAME_OVER;
      break;
    }

    Sleep(p_snake->sleep_time);
    SnakeMove(p_snake);

  } while (p_snake->status == GAME_RUNNING);
}

void GamePause(void) {
  while (true) {
    Sleep(100);

    if (KeyIsPressed(VK_SPACE)) {
      break;
    }
  }
}

void SnakeMove(const Snake_t* const p_snake) {
  SnakeNode_t* p_next = (SnakeNode_t*)malloc(sizeof(SnakeNode_t));

  if (NULL == p_next) {
    perror("SnakeMove()::malloc()");
  }

  p_next->next = NULL;

  switch (p_snake->direction) {
    case UP:
      p_next->x = p_snake->p_head->x;
      p_next->y = p_snake->p_head->y - 1;
      break;
    case DOWN:
      p_next->x = p_snake->p_head->x;
      p_next->y = p_snake->p_head->y + 1;
      break;
    case LEFT:
      p_next->x = p_snake->p_head->x - 2;
      p_next->y = p_snake->p_head->y;
      break;
    case RIGHT:
      p_next->x = p_snake->p_head->x + 2;
      p_next->y = p_snake->p_head->y;
      break;
    default:
      break;
  }

  // 判断蛇头到达的坐标处是否是食物
  if (IsFood(p_snake, p_next)) {
    EatFood(p_snake, p_next);
  } else {
    NoFood(p_snake, p_next);
  }

  KillByWall(p_snake);
  KillBySelf(p_snake);
}

void EatFood(Snake_t* p_snake, SnakeNode_t* p_next) {
  p_next->next = p_snake->p_head;
  p_snake->p_head = p_next;

  // 打印蛇身
  for (SnakeNode_t* p_current = p_snake->p_head; p_current != NULL;
       p_current = p_current->next) {
    SetPos(p_current->x, p_current->y);
    wprintf(L"%lc", BODY);
  }

  free(p_snake->p_food);
  p_snake->score += p_snake->food_weight;

  CreateFood(p_snake);
}

void NoFood(Snake_t* p_snake, SnakeNode_t* p_next) {  // 头插
  p_next->next = p_snake->p_head;
  p_snake->p_head = p_next;

  // 打印蛇身
  SnakeNode_t* p_current = p_snake->p_head;

  while (p_current->next->next != NULL) {
    SetPos(p_current->x, p_current->y);
    wprintf(L"%lc", BODY);
    p_current = p_current->next;
  }

  SetPos(p_current->next->x, p_current->next->y);
  printf("  ");
  free(p_current->next);
  p_current->next = NULL;
}

void KillByWall(Snake_t* p_snake) {
  if (p_snake->p_head->x == 0 || p_snake->p_head->x == 56 ||
      p_snake->p_head->y == 0 || p_snake->p_head->y == 26) {
    p_snake->status = KILL_BY_WALL;
  }
}

void KillBySelf(Snake_t* p_snake) {
  SnakeNode_t* p_current = p_snake->p_head->next;

  while (p_current != NULL) {
    if (p_snake->p_head->x == p_current->x &&
        p_snake->p_head->y == p_current->y) {
      p_snake->status = KILL_BY_SELF;
    }

    p_current = p_current->next;
  }
}

void GameEnd(Snake_t* p_snake) {
  SetPos(20, 12);
  switch (p_snake->status) {
    case END_NORMAL:
      printf("您主动退出游戏\n");
      break;
    case KILL_BY_SELF:
      printf("自杀了，游戏结束\n");
      break;
    case KILL_BY_WALL:
      printf("撞墙了，游戏结束\n");
      break;
    default:
      break;
  }
  
  SetPos(0, 27);

  // 释放节点
  SnakeNode_t* p_current = p_snake->p_head;

  while (p_current != NULL) {
    SnakeNode_t* tmp = p_current;
    p_current = p_current->next;
    free(tmp);
  }

  p_snake->p_head = NULL;
}
