#include <stdlib.h>
#include <stdio.h>
#include <dexterm.h>
#include <unistd.h>

int main()
{
  /*int ch, t, tmp;*/

  terminit();
  printf("%d\n", kbwaiting()); /* testing pipe */

  /*
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
