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