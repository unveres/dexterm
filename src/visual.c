#include <windows.h>
#include "global.h"

/* FUNCTIONS DEFINED IN THIS SOURCE FILE */

int curvis(int);
int cursiz(int);

/* ACTUAL CODE */

static inline CONSOLE_CURSOR_INFO _curvis_cursiz(int vis, int siz)
{
  CONSOLE_CURSOR_INFO cursor_data = { -1, -1 },
                      error       = { -1, -1 };
  int                 to_change   = 0;

  if (!GetConsoleCursorInfo(stdout_handle, &cursor_data))
    return error;

  if (vis != CUR_NOCHANGE) {
    cursor_data.bVisible = vis;
    to_change = 1;
  }

  if (siz != CUR_NOCHANGE) {
    cursor_data.dwSize = siz;
    to_change = 1;
  }

  if (to_change && !SetConsoleCursorInfo(stdout_handle, &cursor_data))
    return error;

  return cursor_data;
}

int curvis(int change)
{
  CONSOLE_CURSOR_INFO cursor_data;

  cursor_data = _curvis_cursiz(change, CUR_NOCHANGE);

  if (cursor_data.bVisible == -1)
    return CUR_ERROR;

  return cursor_data.bVisible;
}

int cursiz(int change)
{
  CONSOLE_CURSOR_INFO cursor_data;

  cursor_data = _curvis_cursiz(CUR_NOCHANGE, change);

  if (cursor_data.bVisible == -1)
    return CUR_ERROR;

  return cursor_data.dwSize;
}
