# dexterm 0.1 for Windows
## what's done?:
- setting/shifting/getting/loading/saving cursor position
- printing according to cursor position
- getting width and height of terminal
- clearing screen (classic clrscr and clrbuf - which clears screen buffer)
- portable stdin clear
- cursor visual appearance functions: changing and getting size and visibility
- unistd usleep and sleep

## known bug:
- putcxy(terminal_width - 1, terminal_height - 1, ch) shifts buffer 1 row down

## TO DO:
- dealing with input
- attributes
- pty
- a lot more, but those above are the mose important
