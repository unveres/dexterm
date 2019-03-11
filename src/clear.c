#include <windows.h>
#include "global.h"

/* FUNCTIONS DEFINED IN THIS SOURCE FILE */

void clrscr   (void);
void clrbuf   (void);

/* ACTUAL CODE */

static inline int _clrreg(int l, int t, int r, int b)
{
  DWORD chars;
  COORD coord;

  coord.X = l;
  coord.Y = t;

  return FillConsoleOutputCharacter(
    stdout_handle,
    ' ',
    (r - l + 1) * (b - t + 1),
    coord,
    &chars
  );
}

void clrscr(void)
{
  CONSOLE_SCREEN_BUFFER_INFO info;
  SMALL_RECT                 sr;

  /* error (WinApi) */
  if (!GetConsoleScreenBufferInfo(stdout_handle, &info))
    return;

  sr = info.srWindow;

  /* error (WinApi/FillConsoleOutputCharacter) */
  if (!_clrreg(sr.Left, sr.Top, sr.Right, sr.Bottom))
    return;

  gotoxy(0, 0);
}

void clrbuf(void)
{
  CONSOLE_SCREEN_BUFFER_INFO info;
  COORD                      coord = { 0, 0 };

  /* error (WinApi) */
  if (!GetConsoleScreenBufferInfo(stdout_handle, &info))
    return;

  if (!_clrreg(0, 0, info.dwSize.X, info.dwSize.Y))
    return;

  SetConsoleCursorPosition(stdout_handle, coord);
}
