#ifndef _DEXTERM_H
#define _DEXTERM_H

#include <stdarg.h>

int  terminit(void);
void termexit(void);
void termreset(void);
void gotoxy(int, int);
void getxy(int *, int *);
void savexy(void);
void loadxy(void);
int  getch(void);
int  getche(void);
int  kbhit(void);
void clrscr(void);
void backspace(void);
int  __dexterm_vscanf(const char *, va_list);
int  __dexterm_scanf(const char *, ...);

#define scanf(__VA_LIST__) __dexterm_scanf(__VA_LIST__)
#define vscanf(__VA_LIST__) __dexterm_vscanf(__VA_LIST__)

#endif
