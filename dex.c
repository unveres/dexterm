#include <stdlib.h>
#include <stdio.h>
#include <dexterm.h>
#include <unistd.h>
#include <sys/ioctl.h>

int main()
{
  int x, y;  

  terminit();

  ioctl(0, FIONREAD, &x);
  printf("%d\n", kbwaiting());
  printf("%d\n", x);

  gotoxy(7,7);
  getxy(&x, &y);

  printf("::%d::%d::\n", x, y);

  

  /*int ch, t, tmp;

  for (;;) {
    for (t = 0; t < 250; ++t) {
      if (kbhit()) {
        if ((ch = getch()) == '\e')
          exit(0);

        putchar(ch);
      }

      usleep(1);
    }
    
    printf(".");
  }*/

  exit(0);
}
