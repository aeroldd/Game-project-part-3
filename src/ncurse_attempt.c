#include <ncurses/ncurses.h>

#include "render/renderer.h"
#include "core/game.h"
#include "world/room.h"

// THIS FILE IS TO TEST THE FOLLOWING
// room generation
// the renderer features

int main() {
    RoomGrid *room = createRoomGrid("room_2.txt");
    return 0;
}