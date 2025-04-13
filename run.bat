@echo off
if exist "build\game.exe" (
    echo Running game.exe...
    cd build
    game.exe
) else (
    echo game.exe not found in build folder.
    echo compile the game first with build_cmake.bat or compile.bat
    pause
)