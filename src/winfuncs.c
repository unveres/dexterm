#include <windows.h>

/* FUNCTIONS DEFINED IN THIS SOURCE FILE */

void usleep(size_t);
void sleep (size_t);

/* ACTUAL CODE */

#define Sleep(MS) Sleep((DWORD)(MS));

void usleep(size_t ms)
{
  Sleep(ms);
}

void sleep(size_t s)
{
  Sleep(1000 * s);
}
