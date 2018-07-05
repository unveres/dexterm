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
static int     has_new_term = 0;
static void   *xy_stack = NULL;

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
  printf("\e[%u;%uH", y, x);
}

void savexy(void)
{
  struct _savexy *new;

  new = malloc(sizeof(struct _savexy));
  new->ptr = xy_stack;
  new->x = term_x;
  new->y = term_y;
  xy_stack = new;
}

void loadxy(void)
{
  struct _savexy *data,
                 *old;

  data = xy_stack;
  old = data->ptr;
  gotoxy(data->x, data->y);
  free(xy_stack);
  xy_stack = old;
}

void getxy(int *x, int *y)
{
  int ch,
      rx
      ry;

  rx = 0;
  ry = 0;

  printf("\e[6n");
  scanf("%*c%*c");

  while ((ch = getchar()) != ';')
    ry = 10 * ry + ch - '0';

  while ((ch = getchar()) != 'R')
    rx = 10 * rx + ch - '0';

  *x = rx;
  *y = ry;
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
  int x, y;

  terminit();

  getxy(&x, &y);
  printf("%d;%d\n", x, y);

  /*for (;;) {
    if (kbhit() > 128)
      break;
  }
  
  printf("%d\n", kbhit());*/
  exit(0);
}
