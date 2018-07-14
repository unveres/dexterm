#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <dexterm.h>

#define dexerror(STR) perror("dexterm: " STR)
#undef scanf
#undef vscanf
#undef printf
#undef vprintf

/*
term_in will be our bufor file, which will store data we want access
through any input function, data stored by term_in is accessed first,
then the data from stdin; it's required when using terminal requests,
response may come later than user input when accessing data asynchronically
so between request and response data is stored in term_in,
some escape sequences may be also stored there (it may be stored in RAM,
and probably should, but it's a nonsense creating two exact interfaces
in two different places, file is more universal than some data structures
in virtual memory - that's why term_in is a file)

pipe is stored in term_in, but I fucking hate this solution,
so I want to do it better soon
*/

struct _savexy {
  struct _savexy *ptr;
  int x;
  int y;
};

FILE                  *term_in;
static struct termios  default_term,
                       new_term;
static int             has_new_term  = 0,
                       kbhit_result  = 0;
static void           *xy_stack      = NULL;

static int flen(FILE *f)
{
  int r, /* returned value */
      p; /* actual cursor position */

  p = ftell(f);
  fseek(f, 0, SEEK_END);
  r = ftell(f);
  fseek(f, p, SEEK_SET); /* return to previous position */
  return r;
}

static void escseq(void)
{
  int ch;

  for (;;) {
    while ((ch = getchar()) != '\e')
      fprintf(term_in, "%c", ch);

    if ((ch = getchar()) == '[')
      return;

    fprintf(term_in, "\e%c", ch);
  }
}

int terminit(void)
{
  int ch;
  /* just resetting terminal to dexterm after 1 terminit call */
  
  if (has_new_term) {
    tcsetattr(STDIN_FILENO, TCSANOW, &new_term);
    return 1;
  }

  if ((term_in = tmpfile()) == NULL) {
    dexerror("terminit/tmpfile");
    exit(EXIT_FAILURE);
  }

  if (!isatty(STDIN_FILENO))
    while ((ch = getchar()) != EOF)
      fprintf(term_in, "%c", ch);

  rewind(term_in);

  /** actual initialization **/

  printf("\e[0m"); /* setting default colors */
  tcgetattr(STDIN_FILENO, &default_term);
  new_term = default_term;
  new_term.c_lflag &= ~(ICANON | ECHO | ISIG);
  new_term.c_cc[VMIN] = 1;
  new_term.c_cc[VTIME] = 0;
  tcsetattr(STDIN_FILENO, TCSANOW, &new_term);
  
  has_new_term = 1;
  atexit(termexit);
  
  return 1;
}

void termexit(void)
{
  if (!has_new_term)
    return;

  tcsetattr(STDIN_FILENO, TCSANOW, &default_term);
  has_new_term = 0;
  printf("\e[0m"); /* restoring previous terminal state */
  fclose(term_in);
}

void termreset(void)
{
  printf("\ec");
}

/*inline void clearin(void)
{
  int i,
      size;

  size = kbwaiting();

  for (i = 0; i < size; ++i)
    getchar();
}*/

/*************************************/
/* CURSOR POSITION-RELATED FUNCTIONS */
/*************************************/

void shiftxy(int x, int y)
{
  if (x > 0)
    printf("\e[%dC", x);
  else if (x < 0)
    printf("\e[%dD", -x);

  if (y > 0)
    printf("\e[%dB", y);
  else if (y < 0)
    printf("\e[%dA", -y);
}

void gotoxy(int x, int y)
{
  int tmpx,
      tmpy;

  if (y == 0) {
    if (x == 0)
      return;

    getxy(&tmpx, &tmpy);
    printf("\e[%u;%uH", tmpy, x);
    return;
  }

  if (x == 0) {
    printf("\e[%uH", y);
    return;
  }

  printf("\e[%u;%uH", y, x);
}

inline void getxy(int *x, int *y)
{
  int ch,
      rx = 0,
      ry = 0;

  printf("\e[6n");
  

  while ((ch = getchar()) != ';')
    ry = 10 * ry + ch - '0';

  while ((ch = getchar()) != 'R')
    rx = 10 * rx + ch - '0';

  *x = rx;
  *y = ry;
}

void savexy(void)
{
  struct _savexy *new;
  int             x,
                  y;

  new = malloc(sizeof(struct _savexy));
  new->ptr = xy_stack;
  getxy(&x, &y);
  new->x = x;
  new->y = y;
  xy_stack = new;
}

void loadxy(void)
{
  struct _savexy *data,
                 *old;

  data = xy_stack;
  old = data->ptr;
  gotoxy(data->x, data->y);
  free(xy_stack);
  xy_stack = old;
}

/***************/
/* BASIC INPUT */
/***************/

int getch(void)
{
  --kbhit_result;
  return getchar();
}

int getche(void)
{
  int r;
  r = getchar();
  putchar(r);
  --kbhit_result;
  return r;
}

/***************/
/* ASYNC INPUT */
/***************/

inline int kbwaiting(void)
{
  int r;
  ioctl(STDIN_FILENO, FIONREAD, &r);
  return r + flen(term_in);
}

inline int kbhit(void)
{
  int tmp;

  tmp = kbhit_result;
  kbhit_result = kbwaiting();
  return !!(kbhit_result - tmp);
}

/*******************/
/* MINOR FUNCTIONS */
/*******************/

void clrscr(void)
{
  printf("\e[H\e[2J");
}

void backspace(void)
{
  printf("\b \b");
}

/***********************/
/* STDIO COMPATIBILITY */
/***********************/

#define NONVSUP(NAME)                         \
int __dexterm_##NAME(const char *format, ...) \
{                                             \
  va_list        args;                        \
  int            r;                           \
                                              \
  va_start(args, format);                     \
  r = __dexterm_v##NAME(format, args);        \
  va_end(args);                               \
                                              \
  return r;                                   \
}

int __dexterm_vscanf(const char *format, va_list args)
{
  struct termios tmp;
  int            r;

  tcgetattr(STDIN_FILENO, &tmp);
  tcsetattr(STDIN_FILENO, TCSANOW, &default_term);
  r = vscanf(format, args);
  tcsetattr(STDIN_FILENO, TCSANOW, &tmp);
  kbhit();
  return r;
}

NONVSUP(scanf);

int __dexterm_vprintf(const char *format, va_list args)
{
  int r;
  r = vprintf(format, args);
  fflush(stdout);
  return r;
}

NONVSUP(printf);
