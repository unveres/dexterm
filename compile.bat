@echo off
if "%1" NEQ "" goto %1
if not exist obj mkdir obj
title Ezechiel's MiniIDE for Windows
set COMPILEFUNCSLOCATION=%~f0
set                 PATH=%PATH%;C:\MinGW\bin
set              LIBNAME=dexterm
set                 OLVL=2
doskey    compile=call %COMPILEFUNCSLOCATION% compile
doskey compilelib=call %COMPILEFUNCSLOCATION% compilelib
doskey  compileex=call %COMPILEFUNCSLOCATION% compileex
cmd

:compilelib
cd obj
gcc -Wall -Wextra -O%OLVL% -I..\include -c ..\src\*.c
cd ..
ar rcs lib%LIBNAME%.a obj\*.o
exit /B

:compileex
gcc -Wall -Wextra -O%OLVL% -Iinclude example.c lib*.a
exit /B

:compile
call :compilelib
call :compileex
exit /B
