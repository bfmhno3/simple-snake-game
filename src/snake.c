#include "snake.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "windows.h"

#define _CRT_SECURE_NO_WARNINGS

/**
 * @brief ��ʼ��Ϸ
 * @param p_snake
 */
void GameStart(Snake_t* p_snake) {
  // ����̨���ڵ�����
  system("mode con cols=100 lines=30");
  system("title ̰����");

  // ���ع��
  HANDLE console_output = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_CURSOR_INFO cursor_info;
  GetConsoleCursorInfo(console_output, &cursor_info);
  cursor_info.bVisible = false;
  SetConsoleCursorInfo(console_output, &cursor_info);

  // ��ӡ��ӭ����
  WelcomeToGame();

  // ������ͼ
  CreateMap();

  // ��ʼ��̰����
  InitSnake(p_snake);
  // getchar();

  // ����ʳ��
  CreateFood(p_snake);
}

void WelcomeToGame(void) {
  SetPos(40, 14);
  printf("��ӭ����̰����С��Ϸ\n");
  SetPos(40, 25);
  system("pause");
  system("cls");
  SetPos(24, 15);
  printf("ʹ�� �� �� �� �� �ֱ�����ߵ��ƶ���F3 �Ǽ��٣�F4 �Ǽ���");
  SetPos(40, 25);
  system("pause");
  system("cls");
}

/**
 * @brief ���ù���λ��
 * @param x ������
 * @param y ������
 */
void SetPos(short x, short y) {
  COORD position;
  position.X = x;
  position.Y = y;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
}

void CreateMap(void) {
  // ��
  SetPos(0, 0);

  for (int i = 0; i <= 56; i += 2) {
    wprintf(L"%lc", WALL);
  }

  // ��
  SetPos(0, 26);

  for (int i = 0; i <= 56; i += 2) {
    wprintf(L"%lc", WALL);
  }

  // ��
  for (int i = 0; i <= 25; i++) {
    SetPos(0, i);
    wprintf(L"%lc", WALL);
  }

  // ��
  for (int i = 0; i <= 25; i++) {
    SetPos(56, i);
    wprintf(L"%lc", WALL);
  }

  // getchar();
}

/**
 * @brief ��ʼ������
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

  // ��ӡ����
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
 * @brief ����ʳ��
 * @param p_snake
 * @note  1. ����Ӧ����������ɵ�
 *        2. ʳ������겻����ǽ��
 *        3. ʳ��� x ��������� 2 �ı���
 *  2 <= x <= 54, 1 <= y <= 25
 */
void CreateFood(Snake_t* p_snake) {
  COORD position;

  // ��֤ x Ϊż��
again:
  do {
    position.Y = rand() % 25 + 1;
    position.X = rand() % 53 + 2;
  } while (position.X % 2 != 0);

  // ���겻�ܺ��ߵ������ͻ
  SnakeNode_t* p_current = p_snake->p_head;

  while (p_current != NULL) {
    // �Ƚ�����
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

  // ��ӡʳ��
  SetPos(p_food->x, p_food->y);
  wprintf(L"%lc", FOOD);
}

void PrintHelpInfo(void) {
  SetPos(60, 14);
  printf("1. ���ܴ�ǽ������ҧ���Լ�");
  SetPos(60, 15);
  printf("2. ʹ�� �� �� �� �� �ֱ�����ߵ��ƶ�");
  SetPos(60, 16);
  printf("3. F3 ���٣�F4 ����");
  SetPos(60, 17);
  printf("4. ESC �˳�, �ո���ͣ��Ϸ");
}

void GameRun(Snake_t* p_snake) {
  PrintHelpInfo();

  do {
    SetPos(60, 9);
    printf("�÷֣�%05d", p_snake->score);
    SetPos(60, 10);
    printf("ÿ��ʳ��ķ�����%2d", p_snake->food_weight);

    if (KeyIsPressed(VK_UP) && p_snake->direction != DOWN) {
      p_snake->direction = UP;
    } else if (KeyIsPressed(VK_DOWN) && p_snake->direction != UP) {
      p_snake->direction = DOWN;
    } else if (KeyIsPressed(VK_LEFT) && p_snake->direction != RIGHT) {
      p_snake->direction = LEFT;
    } else if (KeyIsPressed(VK_RIGHT) && p_snake->direction != LEFT) {
      p_snake->direction = RIGHT;
    } else if (KeyIsPressed(VK_F3)) {  // ����
      if (p_snake->sleep_time >= 80) {
        p_snake->sleep_time -= 30;
        p_snake->food_weight += 2;
      }
    } else if (KeyIsPressed(VK_F4)) {  // ����
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

  // �ж���ͷ��������괦�Ƿ���ʳ��
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

  // ��ӡ����
  for (SnakeNode_t* p_current = p_snake->p_head; p_current != NULL;
       p_current = p_current->next) {
    SetPos(p_current->x, p_current->y);
    wprintf(L"%lc", BODY);
  }

  free(p_snake->p_food);
  p_snake->score += p_snake->food_weight;

  CreateFood(p_snake);
}

void NoFood(Snake_t* p_snake, SnakeNode_t* p_next) {  // ͷ��
  p_next->next = p_snake->p_head;
  p_snake->p_head = p_next;

  // ��ӡ����
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
      printf("�������˳���Ϸ\n");
      break;
    case KILL_BY_SELF:
      printf("��ɱ�ˣ���Ϸ����\n");
      break;
    case KILL_BY_WALL:
      printf("ײǽ�ˣ���Ϸ����\n");
      break;
    default:
      break;
  }
  
  SetPos(0, 27);

  // �ͷŽڵ�
  SnakeNode_t* p_current = p_snake->p_head;

  while (p_current != NULL) {
    SnakeNode_t* tmp = p_current;
    p_current = p_current->next;
    free(tmp);
  }

  p_snake->p_head = NULL;
}
