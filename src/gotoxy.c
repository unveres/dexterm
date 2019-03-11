#include <windows.h>
#include <stdlib.h>
#include "global.h"

#include <stdio.h>

/* FUNCTIONS DEFINED IN THIS SOURCE FILE */

void gotoxy (int,   int);
void getxy  (int *, int *);
void shiftxy(int,   int);
void savexy (void);
void loadxy (void);
void getwh  (int *, int *);

/* ACTUAL CODE */

typedef struct SAVEXY_DATA {
  struct SAVEXY_DATA *ptr;
  int                 x;
  int                 y;
} SAVEXY_DATA;

SAVEXY_DATA *stack = NULL;

static inline void popstack(void)
{
  SAVEXY_DATA *n;  /* previous stack node */

  n     = stack;
  stack = stack->ptr;
  free(n);
}

inline void gotoxy(int x, int y)
{
  CONSOLE_SCREEN_BUFFER_INFO info;
  COORD                      coord;

  if (x < 0 || y < 0)
    return;

  /* error (WinApi) */
  if (!GetConsoleScreenBufferInfo(stdout_handle, &info))
    return;

  coord.X = info.srWindow.Left + x;
  coord.Y = info.srWindow.Top + y;

  if (coord.X > info.srWindow.Right || coord.Y > info.srWindow.Bottom)
    return;

  SetConsoleCursorPosition(stdout_handle, coord);
}

inline void getxy(int *x, int *y)
{
  CONSOLE_SCREEN_BUFFER_INFO info;

  /* error (WinApi) */
  if (!GetConsoleScreenBufferInfo(stdout_handle, &info))
    return;

  *x = info.dwCursorPosition.X - info.srWindow.Left;
  *y = info.dwCursorPosition.Y - info.srWindow.Top;
}

void shiftxy(int dx, int dy)
{
  int x = -1,
      y = -1;

  getxy(&x, &y);
  
  /* error (getxy) */
  if (x == -1)
    return;

  gotoxy(x + dx, y + dy);
}

void savexy(void)
{
  SAVEXY_DATA *n;       /* new stack node */
  int          x  = -1,
               y  = -1;

  /* error (if no free memory or other malloc error) */
  if ((n = malloc(sizeof(SAVEXY_DATA))) == NULL)
    return;

  getxy(&x, &y);

  /* error (getxy) */
  if (x == -1) {
    free(n);
    return;
  }

  n->x   = x;
  n->y   = y;
  n->ptr = stack;
  stack  = n;
}

void loadxy(void)
{
  if (stack == NULL)
    return;

  gotoxy(stack->x, stack->y);
  popstack();
}

void getwh(int *w, int *h)
{
  CONSOLE_SCREEN_BUFFER_INFO info;

  /* error (WinApi) */
  if (!GetConsoleScreenBufferInfo(stdout_handle, &info))
    return;

  *w = info.srWindow.Right - info.srWindow.Left + 1;
  *h = info.srWindow.Bottom - info.srWindow.Top + 1;
}

void __free_gotoxy_c(void)
{
  while (stack)
    popstack();
}
