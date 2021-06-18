# Isometric tile/game engine

## Compilation

`gcc --std=c99 -Wall -pedantic -lSDL2 -lSDL2_ttf -lSDL2_mixer -lSDL2_image data.c actor.c animation.c accent.c iso.c ui.c  timer.c rendering.c serialization.c queue.c tile.c -g -o iso`

## Todo

1. Flexible viewport (done)
2. Mouse translation (done)
3. Zoom / Clipping (done)
4. Entities (done)
5. Serialization (save board state to file and load from file) (done)
6. Animations / For tiles (water) and entities (done)
7. Swap between iso and standard grid
