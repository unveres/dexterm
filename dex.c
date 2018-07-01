#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>

struct _savexy {
  struct _savexy *ptr;
  int x;
  int y;
};

static struct  termios default_term,
               new_term;
static int     has_new_term = 0,
               term_x = -1,
               term_y = -1;
static void   *_savexy = NULL;

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

void gotoxy(int x, int y)
{
  term_x = x;
  term_y = y; 
  printf("\e[%u;%uH", y, x);
}

void savexy(void)
{
  struct _savexy *new;

  new = malloc(sizeof(struct _savexy));
  new->ptr = _savexy;
  new->x = term_x;
  new->y = term_y;
  _savexy = new;
}

void loadxy(void)
{
  struct _savexy *data,
                 *old;

  data = _savexy;
  old = data->ptr;
  gotoxy(data->x, data->y);
  free(_savexy);
  _savexy = old;
}

void getxy(int *x, int *y)
{
  *x = term_x;
  *y = term_y;
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
