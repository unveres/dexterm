#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <termspec.h>

struct _savexy {
  struct _savexy *ptr;
  int x;
  int y;
};

static int _gotoxy[2] = { -1, -1 };
static void *_savexy = NULL;

void gotoxy(int x, int y)
{
  _gotoxy[0] = x;
  _gotoxy[1] = y; 
  printf("\e[%u;%uH", y, x);
}

void savexy(void)
{
  struct _savexy *new = malloc(sizeof(struct _savexy));
  new->ptr = _savexy;
  new->x = _gotoxy[0];
  new->y = _gotoxy[1];
  _savexy = new;
}

void loadxy(void)
{
  struct _savexy *data = ((struct _savexy *)_savexy);
  void *old = data->ptr;
  gotoxy(data->x, data->y);
  free(_savexy);
  _savexy = old;
}

int *getxy(void)
{
  return _gotoxy;
}

static int _getch(int echo)
{
  struct termios old, new;
  int r;

  tcgetattr(0, &old);
  new = old;
  new.c_lflag &= ~ICANON;
  new.c_lflag &= echo ? ECHO : ~ECHO;
  tcsetattr(0, TCSANOW, &new);
  r = getchar();
  tcsetattr(0, TCSANOW, &old);
    
  return r;
}

int getch(void)
{
  return _getch(0);
}

int getche(void)
{
  return _getch(1);
}

