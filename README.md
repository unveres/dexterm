# dexterm
## what's done?:
- resetting terminal
- getting/setting/saving/loading cursor position
- asynchronous read from stdin (getch, getche, kbhit)
-  clearing screen

## to do
- support for pipes
- scrolling
- erasing functions other than clrscr
- colors / styling and saving it
- getch escaping all special key codes
- support for wchat_t
- detecting width/height of screen
- creating parallel terminal "session" (just like man does)
- disabling/enabling cursor visibility

low priority:
- some other terminal special combos
- printer support
- support of more terminals than just VT-100
- terminal detection, portability between OSes and different terminals
