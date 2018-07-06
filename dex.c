#include <stdio.h>
#include <dexterm.h>

int main()
{
  int x, y;

  terminit();
  getxy(&x, &y);
  printf("%d;%d\n", x, y);

  /*for (;;) {
    if (kbhit() > 128)
      break;
  }
  
  printf("%d\n", kbhit());*/
  termexit();
  return 0;
}
