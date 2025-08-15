#include "test.h"

#include <stdio.h>

#include "snake.h"

void test(void) {
  char choice = 'N';

  do {
    // 创建贪吃蛇
    Snake_t snake = {0};

    // 1. 游戏开始，初始化游戏
    GameStart(&snake);

    // 2. 游戏运行，游戏的正常运行过程
    GameRun(&snake);
    // getchar();

    // 3. 游戏结束，游戏善后（释放资源）
    GameEnd(&snake);

    // 是否再来
    SetPos(20, 18);
    printf("再来一局吗？(Y/N)：");
    SetPos(20, 19);
    (void)scanf(" %c", &choice);
    SetPos(0, 26);
  } while ('Y' == choice || 'y' == choice);
}
