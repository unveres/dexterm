#include <stdio.h>
#include <dexterm.h>

/*
what's done?:
  resetting terminal
  getting/setting/saving/loading cursor position
  asynchronous read from stdin (getch, getche, kbhit)
  clearing screen
*/

int main()
{
  int x, y;

  terminit();
  getxy(&x, &y);
  printf("%d;%d\n", x, y);
  scanf("%*s");
  shiftxy(7, -1);
  termexit();
  return 0;
}


