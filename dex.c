#include <stdlib.h>
#include <stdio.h>
#include <dexterm.h>

int main()
{
  int x, y;

  terminit();
  getxy(&x, &y);
  printf("%d;%d\n", x, y);
  scanf("%*s");
  exit(0);
}
