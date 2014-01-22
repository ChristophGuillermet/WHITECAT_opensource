@echo off

gcc -O2 cbuild.c -o %TEMP%\cbuild.exe
if not errorlevel 0 goto enderror

%TEMP%\cbuild.exe %1 %2 %3 %4 %5

del %TEMP%\cbuild.exe
goto end

:enderror
echo Problems with building cbuild.exe (build manually)

:end
