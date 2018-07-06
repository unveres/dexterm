#include <stdlib.h>
#include <stdio.h>
#include <dexterm.h>
#include <unistd.h>

/* prime example of how "worse is better" works */

int async_find(const char *str)
{
  FILE *tmp;
  int   ch,
        i,
        r;

  i = 0;

  if ((tmp = tmpfile()) == NULL) {
    perror("tmpfile");
    exit(EXIT_FAILURE);
  }

  while ((ch = fgetc(stdin)) != EOF) {
    if (ch == str[i])
      ++i;
    else
      i = 0;

    fprintf(tmp, "%c", ch);

    if (str[i] == '\0')
      break;
  }

  if (str[i] != '\0')
    i = 0;

  r = !!i;

  i = ftell(tmp) - i;                 /* i don't like this line */
  fseek(tmp, i, SEEK_SET);            /* i don't like this line */
  
  do {
    fseek(tmp, --i, SEEK_SET);        /* i don't like this line */
    fscanf(tmp, "%c", (char*)&ch);
    ungetc(ch, stdin);
  } while (i != 0);                   /* i don't like this line */

  fclose(tmp);
  return r; 
}

int main()
{
  int ch;

  if (async_find("XDDDD"))
    printf("znalazlo to XDDDD XD\n");

  while ((ch = fgetc(stdin)) != EOF)
    printf("%c", ch);

  exit(0);
}
