#include "main.h"

#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
  // ���ñ��ػ���
  setlocale(LC_ALL, "");
  srand((unsigned int)time(NULL));
  test();
  return 0;
}
