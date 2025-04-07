# Define the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Define the project name and version
project(GameProject VERSION 1.0)

# Set the C standard
set(CMAKE_C_STANDARD 99)

# Define the executable name and source files
set(SOURCE_FILES
    src/main.c
    src/core/game.h
    src/dialogue/dialogue.c src/dialogue/dialogue.h
    src/entity/entity.c src/entity/entity.h
    src/entity/monster.c src/entity/monster.h
    src/entity/pathfinding.c src/entity/pathfinding.h
    src/entity/player.c src/entity/player.h
    src/gameplay/combat.c src/gameplay/combat.h
    src/gameplay/initiative.c src/gameplay/initiative.h
    src/gameplay/menus.c src/gameplay/menus.h
    src/item/item.c src/item/item.h
    src/util/input.c src/util/input.h src/util/util.c src/util/util.h
    src/world/map.c src/world/map.h src/world/room.c src/world/room.h
    src/world/camera.c src/world/camera.h
)

# Include directories for headers
include_directories(
    src/core
    src/dialogue
    src/entity
    src/gameplay
    src/item
    src/util
    src/world
)

# Define the executable
add_executable(game ${SOURCE_FILES})

# If you have additional settings (e.g., linking external libraries), add them here:
# target_link_libraries(GameProject SomeLibrary)