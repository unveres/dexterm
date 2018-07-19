#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <dexterm.h>

#undef scanf
#undef vscanf
#undef printf
#undef vprintf

#define dexerror(STR) do { \
  perror("dexterm: " STR); \
  exit(EXIT_FAILURE);      \
} while(0)

struct _savexy {
  struct _savexy *ptr;
  int x;
  int y;
};

FILE                  *term_in;
static struct termios  default_term,
                       new_term;
static int             has_new_term  = 0,
                       kbhit_result  = 0,
                       pipe_pipe[2]  = { -1 },
                       term_pipe[2]  = { -1 };
static void           *xy_stack      = NULL;

int terminit(void)
{
  pid_t pid;
  int   ch,
        tmp;
  char  buffer[BUFSIZ];

  /* just resetting terminal to dexterm after 1 terminit call */
  
  if (has_new_term) {
    tcsetattr(STDIN_FILENO, TCSANOW, &new_term);
    return 1;
  }

  /* creating process for pipe support */

  do {
    if (!isatty(STDIN_FILENO)) {
      ch = getchar();

      if (ch == EOF)
        break; /* if pipe is empty we don't need that process */

      ungetc(ch, stdin);
      pipe(pipe_pipe);
      pid = fork();

      if (pid == -1)
        dexerror("terminit/fork");

      if (pid == 0) {
        do {
          read(pipe_pipe[0], &ch, 1);
          ch = getchar();
          write(pipe_pipe[1], &ch, 1);
        } while(ch != EOF);

        exit(EXIT_SUCCESS);
      }

      stdin = freopen(NULL, "r", stdin);
    }
  } while(0);

  /* creating process for terminal requests support */

  pipe(term_pipe);
  pid = fork();

  if (pid == -1)
    dexerror("terminit/fork");

  if (pid == 0) {
    do {
      /* outputing text */
      read(term_pipe[0], &ch, 1);

      if (ch == EOF)
        break;

      putchar(ch);

      /* support for terminal requests */
      ioctl(STDIN_FILENO, FIONREAD, &tmp);

      while (tmp != 0) {
        tmp -= read(STDIN_FILENO, buffer, sizeof(buffer));
        write(term_pipe[1], buffer, sizeof(buffer));
      }
    } while (1);

    exit(EXIT_SUCCESS);
  }

  /* setting default colors */

  printf("\e[0m");

  /* setting terminal for asynchronous read */

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

  /* restoring previous terminal state */

  tcsetattr(STDIN_FILENO, TCSANOW, &default_term);
  has_new_term = 0;

  /* restoring default colors */

  printf("\e[0m"); 
}

void termreset(void)
{
  printf("\ec");
}

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

static int term_pipe_getchar()
{
  char r;

  if (read(term_pipe[0], &r, 1))
    return r;

  return EOF;
}

inline void getxy(int *x, int *y)
{
  int   rx = 0,
        ry = 0,
        ch;

  write(term_pipe[1], "\e[6n", 4);
  term_pipe_getchar();
  term_pipe_getchar();

  while ((ch = term_pipe_getchar()) != ';')
    ry = 10 * ry + ch - '0';

  while ((ch = term_pipe_getchar()) != 'R')
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
  r = getch();
  putchar(r);
  return r;
}

/***************/
/* ASYNC INPUT */
/***************/

inline int kbwaiting(void)
{
  int r;
  ioctl(STDIN_FILENO, FIONREAD, &r);
  return r;
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
