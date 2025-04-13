@echo off
mkdir build
gcc src/gameplay/combat.c src/gameplay/initiative.c src/render/shadowcasting.c src/dialogue/dialogue.c src/ncurse_attempt.c src/item/item.c src/entity/entity_stats.c src/entity/player.c src/util/input.c src/render/renderer.c src/world/room.c src/entity/entity.c src/util/util.c src/item/inventory.c -I src/core/ -lncurses -DNCURSES_STATIC  -o build/ncurse_test.exe
echo "COMPILED THE NCURSE TEST"
