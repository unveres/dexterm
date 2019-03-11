#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#include "global.h"

/* FUNCTIONS DEFINED IN THIS SOURCE FILE */

int  vprintfxy(int, int, const char *, va_list);
int  printfxy (int, int, const char *, ...);
int  putcxy   (int, int, int);
void flushin  (void);

/* ACTUAL CODE */

int vprintfxy(int x, int y, const char *format, va_list args)
{
  int r;  /* returned value */

  savexy();
  gotoxy(x, y);
  r = vprintf(format, args);
  loadxy();

  return r;
}

int putcxy(int x, int y, int ch)
{
  int r;  /* returned value */

  savexy();
  gotoxy(x, y);
  r = putchar(ch);
  loadxy();

  return r;
}

int printfxy(int x, int y, const char *format, ...)
{
  va_list args;
  int     r;     /* returned value */

  va_start(args, format);
  r = vprintfxy(x, y, format, args);
  va_end(args);

  return r;
}

void flushin(void)
{
  FlushConsoleInputBuffer(stdin_handle);
}
