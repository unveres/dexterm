#ifndef _DEXTERM_H
#define _DEXTERM_H

int  terminit(void);
void termexit(void);
void termreset(void);
void clrscr(void);
void backspace(void);
void gotoxy(int x, int y);
void getxy(int *x, int *y);
void savexy(void);
void loadxy(void);
int  getch(void);
int  getche(void);
int  kbhit(void);
int  __dexterm_scanf(const char *format, ...);

#define scanf(__VA_LIST__) __dexterm_scanf(__VA_LIST__)

#endif
