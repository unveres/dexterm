#include <windows.h>
#include "global.h"

/* FUNCTIONS DEFINED IN THIS SOURCE FILE */

void terminit       (void);
void termexit       (void);

/* FUNCTIONS FROM OTHER HEADERS */

void __free_gotoxy_c(void);

/* ACTUAL CODE */

HWND  stdin_handle            = NULL,
      stdout_handle           = NULL;
int   is_terminal_initialized = 0,
      default_curvis_value,
      default_cursiz_value;

void terminit(void)
{
  if (is_terminal_initialized)
    return;

  is_terminal_initialized = 1;
  stdin_handle            = GetStdHandle(STD_INPUT_HANDLE);
  stdout_handle           = GetStdHandle(STD_OUTPUT_HANDLE);
  default_curvis_value    = curvis(CUR_NOCHANGE);
  default_cursiz_value    = cursiz(CUR_NOCHANGE);

}

void termexit(void)
{
  curvis(default_curvis_value);
  cursiz(default_cursiz_value);
  __free_gotoxy_c();
  is_terminal_initialized = 0;
}
