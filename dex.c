#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <setjmp.h>

struct termios default_term,
               new_term;

int            has_new_term = 0;

void termexit(void)
{
  if (has_new_term) {
    tcsetattr(0, TCSANOW, &default_term);
    has_new_term = 0;
  }
}

void terminit(void)
{
  if (has_new_term) {
    tcsetattr(0, TCSANOW, &new_term);
    return;
  }

  tcgetattr(0, &default_term);
  new_term = default_term;
  new_term.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(0, TCSANOW, &new_term);
  has_new_term = 1;
  atexit(termexit);
}

int getch(void)
{
  return getchar();
}

int getche(void)
{
  int r;
  r = getchar();
  putchar(r);
  return r;
}

inline int kbhit(void)
{
  int r;
  ioctl(0, FIONREAD, &r);
  return r;
}

int main()
{
  int n;
  terminit();

  for (;;)
    if (kbhit() > 128)
      break;
  
  printf("%d\n", kbhit());
  termexit();
  return 0;
}
