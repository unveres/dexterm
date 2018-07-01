#ifndef _TERMSPEC_H
#define _TERMSPEC_H

#define CDEFAULT        "\e[0m"
#define CBOLD           "\e[1m"
#define CDIM            "\e[2m"
#define CSTANDOUT       "\e[3m"
#define CUNDERSCORE     "\e[4m"
#define CBLINK          "\e[5m"
#define CREVERSE        "\e[7m"
#define CHIDDEN         "\e[8m"
#define CDEFBOLD        "\e[21m"
#define CDEFDIM         "\e[22m"
#define CDEFSTANDOUT    "\e[23m"
#define CDEFUNDERSCORE  "\e[24m"
#define CDEFBLINK       "\e[25m"
#define CDEFREVERSE     "\e[27m"
#define CDEFHIDDEN      "\e[28m"
#define FBLACK          "\e[30m"
#define FRED            "\e[31m"
#define FGREEN          "\e[32m"
#define FYELLOW         "\e[33m"
#define FBLUE           "\e[34m"
#define FMAGENTA        "\e[35m"
#define FCYAN           "\e[36m"
#define FLIGHTGREY      "\e[37m"
#define FDEFAULT        "\e[39m"
#define BBLACK          "\e[40m"
#define BRED            "\e[41m"
#define BGREEN          "\e[42m"
#define BYELLOW         "\e[43m"
#define BBLUE           "\e[44m"
#define BMAGENTA        "\e[45m"
#define BCYAN           "\e[46m"
#define BLIGHTGREY      "\e[47m"
#define BDEFAULT        "\e[49m"
#define FGREY           "\e[90m"
#define FLIGHTRED       "\e[91m"
#define FLIGHTGREEN     "\e[92m"
#define FLIGHTYELLOW    "\e[93m"
#define FLIGHTBLUE      "\e[94m"
#define FLIGHTMAGENTA   "\e[95m"
#define FLIGHTCYAN      "\e[96m"
#define FWHITE          "\e[97m"
#define BGREY           "\e[100m"
#define BLIGHTRED       "\e[101m"
#define BLIGHTGREEN     "\e[102m"
#define BLIGHTYELLOW    "\e[103m"
#define BLIGHTBLUE      "\e[104m"
#define BLIGHTMAGENTA   "\e[105m"
#define BLIGHTCYAN      "\e[106m"
#define BWHITE          "\e[107m"
#define CLREND          "\e[K"
#define CLRBEG          "\e[1K"
#define CLRLN           "\e[2K"
#define CVISIBLE        "\e[?25h"
#define CINVISIBLE      "\e[?25l"
#define CDEL            "\b \b"
#define CHOME           "\e[H"
#define PAGE            "\e[2J"
#define NEWPAGE         "\r" PAGE
#define CLRSCR          CHOME PAGE
#define terminit()      printf("\e7")
#define termexit()      printf(CDEFAULT "\e8")

void gotoxy(int x, int y);
void savexy(void);
void loadxy(void);
int *getxy(void);

int getch(void);
int getche(void);

#endif
