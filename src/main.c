#include "main.h"

#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
  // 设置本地环境
  setlocale(LC_ALL, "");
  srand((unsigned int)time(NULL));
  test();
  return 0;
}
