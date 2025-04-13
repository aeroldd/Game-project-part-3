@echo off
cd /d "%~dp0build"
mingw32-make
echo Running my_game.exe in: %cd%
game.exe
pause