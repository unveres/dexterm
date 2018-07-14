#ifndef _DEXTERM_H
#define _DEXTERM_H

#include <stdarg.h>

int  terminit(void);
void termexit(void);
void termreset(void);
void shiftxy(int, int);
void gotoxy(int, int);
void getxy(int *, int *);
void savexy(void);
void loadxy(void);
int  getch(void);
int  getche(void);
int  kbwaiting(void);
int  kbhit(void);
/*void clearin(void);*/
void clrscr(void);
void backspace(void);
int  __dexterm_vscanf(const char *, va_list);
int  __dexterm_scanf(const char *, ...);
int  __dexterm_vprintf(const char *, va_list);
int  __dexterm_printf(const char *, ...);

#define printf(...) __dexterm_printf(__VA_ARGS__)
#define vprintf(...) __dexterm_vprintf(__VA_ARGS__)
#define scanf(...) __dexterm_scanf(__VA_ARGS__)
#define vscanf(...) __dexterm_vscanf(__VA_ARGS__)

#endif
