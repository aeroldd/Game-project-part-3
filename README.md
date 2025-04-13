# dungeon.txt - A text-based dungeon crawler inspired by D&D and Rogue #

LINK TO VIDEO: https://youtu.be/_WDc3PL-Jec

## Features ##
- Room exploration with fog of war using a shadowcasting algorithm
- Turn based tactical combat system
- Monster AI pathfinding using breadth-first search
- A modular dialogue system
- Starting menu and character creation
- Designable rooms, items and monsters with text files

## CONTROLS ##
[WASD] Player movement
[1] Attack
(In attack menu) [1] Confirm target [space] Cycle through targets [x] Exit menu
[2] Inventory 
(In inventory menu) [1-4] Items [5] Use item [6] Drop item [x] Exit menu
[3] Enter door
[0] Skip turn

[any key] Skip through dialogue

## !! Only works on Windows (for now). Future versions will be cross compatible !! ##

## Build instructions ##
### Requirements ###
Windows (future support for other systems will be implemented in the future)
GCC compiler
CMake (Recommended)

With CMake (recommended if CMake is installed, as well as MinGW-make) for windows
1) Create a build folder
2) run build_cmake
3) Game will be output in the /build/ directory

With GCC
1) Run compile.bat
--OR--
Paste this command:
@echo off
REM Check if the build folder exists
IF NOT EXIST build (
    mkdir build
)

REM Compile the project
gcc -std=c99 -o build\game.exe ^
    src\main.c ^
    src\dialogue\dialogue.c ^
    src\entity\entity.c ^
    src\entity\monster.c ^
    src\entity\pathfinding.c ^
    src\entity\player.c ^
    src\entity\entity_runner.c ^
    src\entity\entity_stats.c ^
    src\gameplay\combat.c ^
    src\gameplay\initiative.c ^
    src\gameplay\menus.c ^
    src\item\item.c ^
    src\item\inventory.c ^
    src\util\input.c ^
    src\util\util.c ^
    src\world\map.c ^
    src\world\room.c ^
    src\world\camera.c ^
    src/render/shadowcasting.c ^
    -Isrc\core ^
    -Isrc\dialogue ^
    -Isrc\entity ^
    -Isrc\gameplay ^
    -Isrc\item ^
    -Isrc\util ^
    -Isrc\world

REM Output result
IF %ERRORLEVEL% EQU 0 (
    echo Build succeeded.
) ELSE (
    echo Build failed.
)

pause

Enjoy!
