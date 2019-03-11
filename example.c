#include <stdio.h>
#include <string.h>
#include <dexterm.h>

int main()
{
  char str[] = "This text is centered";
  int  w,
       h,
       x,
       y;

  terminit();

  curvis(CUR_HIDE); /* hiding cursor */
  getwh(&w, &h);    /* saving width and height of terminal window */

  savexy();
  printf("fooo");   /* fooo */
  shiftxy(-1, 0);
  printf("bar");    /* foobar */
  loadxy();
  printf("F");      /* Foobar */
  shiftxy(-1, 1);
  puts("Waiting 3 seconds...");
  sleep(3);
  putcxy(1,     1    , 219);
  putcxy(w - 2, 1    , 219);
  putcxy(1,     h - 2, 219);
  putcxy(w - 2, h - 2, 219);
  sleep(3);
  printf("Width and height of terminal: %d:%d\n", w, h);
  getxy(&x, &y);
  printf("Cursor position:              %d:%d\r", x, y);
  curvis(CUR_SHOW);
  sleep(3);
  curvis(CUR_HIDE);
  shiftxy((w - strlen(str)) / 2, 1);
  printf("%s\r\n", str);
  printf("Now it's time for clrscr and gotoxy");
  sleep(3);
  clrscr();
  gotoxy(0, h / 2);
  printf("That's it, just one more time clrscr, goodbye!");
  sleep(3);
  clrscr();
  termexit();
  return 0;
}
