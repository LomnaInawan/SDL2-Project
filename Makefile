start:
	gcc -g ./src/core.c ./src/draw.c ./src/basic-collision.c ./src/game.c ./src/text.c ./src/audio.c -I ./include -o ./build/test -lSDL2main -lSDL2 -lSDL2_ttf 
