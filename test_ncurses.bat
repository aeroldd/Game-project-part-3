@echo off
gcc -o build/ncurse_test.exe src/render/shadowcasting.c src/dialogue/dialogue.c src/ncurse_attempt.c src/item/item.c src/entity/entity_stats.c src/entity/player.c src/util/input.c src/render/renderer.c src/world/room.c src/entity/entity.c src/util/util.c -I src/core/ -lncurses -DNCURSES_STATIC  
echo "COMPILED THE NCURSE TEST"
cd build
start ncurse_test