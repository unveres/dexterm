#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <dexterm.h>

#undef scanf
#undef vscanf

struct _savexy {
  struct _savexy *ptr;
  int x;
  int y;
};

static struct termios  default_term,
                       new_term;
static int             has_new_term  = 0,
                       kbhit_result  = 0;
static void           *xy_stack      = NULL;

int terminit(void)
{
  /* just resetting terminal to dexterm after 1 terminit call */
  if (has_new_term) {
    tcsetattr(0, TCSANOW, &new_term);
    return 1;
  }

  /** actual initialization **/

  printf("\e[0m"); /* setting default colors */
  tcgetattr(0, &default_term);
  new_term = default_term;
  new_term.c_lflag &= ~(ICANON | ECHO);
  new_term.c_cc[VMIN] = 1;
  new_term.c_cc[VTIME] = 0;
  tcsetattr(0, TCSANOW, &new_term);
  
  has_new_term = 1;
  atexit(termexit);
  
  return 1;
}

void termexit(void)
{
  if (!has_new_term)
    return;

  tcsetattr(0, TCSANOW, &default_term);
  has_new_term = 0;
  printf("\e[0m"); /* restoring previous terminal state */
}

void termreset(void)
{
  printf("\ec");
}

inline void clearin(void)
{
  int i,
      size;

  size = kbwaiting();

  for (i = 0; i < size; ++i)
    getchar();
}

/*************************************/
/* CURSOR POSITION-RELATED FUNCTIONS */
/*************************************/

void shiftxy(int x, int y)
{
  if (x > 0)
    printf("\e[%dC", x);
  else if (x < 0)
    printf("\e[%dD", -x);

  if (y > 0)
    printf("\e[%dB", y);
  else if (y < 0)
    printf("\e[%dA", -y);
}

void gotoxy(int x, int y)
{
  int tmpx,
      tmpy;

  if (y == 0) {
    if (x == 0)
      return;

    getxy(&tmpx, &tmpy);
    printf("\e[%u;%uH", tmpy, x);
    return;
  }

  if (x == 0) {
    printf("\e[%uH", y);
    return;
  }

  printf("\e[%u;%uH", y, x);
}

inline void getxy(int *x, int *y)
{
  int ch,
      rx = 0,
      ry = 0;

  clearin();
  printf("\e[6n");
  scanf("%*c%*c");

  while ((ch = getchar()) != ';')
    ry = 10 * ry + ch - '0';

  while ((ch = getchar()) != 'R')
    rx = 10 * rx + ch - '0';

  *x = rx;
  *y = ry;
}

void savexy(void)
{
  struct _savexy *new;
  int             x,
                  y;

  new = malloc(sizeof(struct _savexy));
  new->ptr = xy_stack;
  getxy(&x, &y);
  new->x = x;
  new->y = y;
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

/***************/
/* BASIC INPUT */
/***************/

int getch(void)
{
  --kbhit_result;
  return getchar();
}

int getche(void)
{
  int r;
  r = getchar();
  putchar(r);
  --kbhit_result;
  return r;
}

/***************/
/* ASYNC INPUT */
/***************/

inline int kbwaiting(void)
{
  int r;
  ioctl(0, FIONREAD, &r);
  return r;
}

inline int kbhit(void)
{
  int tmp;

  tmp = kbhit_result;
  kbhit_result = kbwaiting();
  return !!(kbhit_result - tmp);
}

/*******************/
/* MINOR FUNCTIONS */
/*******************/

void clrscr(void)
{
  printf("\e[H\e[2J");
}

void backspace(void)
{
  printf("\b \b");
}

/***********************/
/* STDIO COMPATIBILITY */
/***********************/

int __dexterm_vscanf(const char *format, va_list args)
{
  struct termios tmp;
  int            r;

  tcgetattr(0, &tmp);
  tcsetattr(0, TCSANOW, &default_term);
  r = vscanf(format, args);
  tcsetattr(0, TCSANOW, &tmp);
  kbhit();
  return r;
}

int __dexterm_scanf(const char *format, ...)
{
  va_list        args;
  int            r;

  va_start(args, format);
  r = __dexterm_vscanf(format, args);
  va_end(args);

  return r;
}
