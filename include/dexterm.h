#ifndef _DEXTERM_H
#define _DEXTERM_H
#include <stddef.h>
#include <stdarg.h>

/* dexterm */
void terminit (void);
void termexit (void);

/* xy axis */
void gotoxy   (int,   int);
void getxy    (int *, int *);
void shiftxy  (int,   int);
void savexy   (void);
void loadxy   (void);
void getwh    (int *, int *);

/* input/output */
int  vprintfxy(int, int, const char *, va_list);
int  printfxy (int, int, const char *, ...);
int  putcxy   (int, int, int);
void flushin  (void);

/* clearing */
void clrscr   (void);
void clrbuf   (void);

/* visual appearance */
int  curvis   (int);
int  cursiz   (int);

/* Windows-specific functions */
void usleep   (size_t);
void sleep    (size_t);

/* MACROS */
#define CUR_ERROR    (-1)
#define CUR_NOCHANGE (-1)
#define CUR_HIDE     (0)
#define CUR_SHOW     (1)

#endif
