#include "test.h"

#include <stdio.h>

#include "snake.h"

void test(void) {
  char choice = 'N';

  do {
    // ����̰����
    Snake_t snake = {0};

    // 1. ��Ϸ��ʼ����ʼ����Ϸ
    GameStart(&snake);

    // 2. ��Ϸ���У���Ϸ���������й���
    GameRun(&snake);
    // getchar();

    // 3. ��Ϸ��������Ϸ�ƺ��ͷ���Դ��
    GameEnd(&snake);

    // �Ƿ�����
    SetPos(20, 18);
    printf("����һ����(Y/N)��");
    SetPos(20, 19);
    (void)scanf(" %c", &choice);
    SetPos(0, 26);
  } while ('Y' == choice || 'y' == choice);
}
