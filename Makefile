CFLAGS = -lSDL2 -lSDL2_ttf -Ilib -lSDL2_image -lpthread
OBJ_PATH = obj
OBJ_FILES = $(wildcard $(OBJ_PATH)/*.o)

output: $(OBJ_PATH) obj/main.o obj/events.o obj/render.o obj/collisions.o obj/data.o obj/audio.o
	gcc $(OBJ_FILES) -o bin/breakout $(CFLAGS) 

$(OBJ_PATH):
	mkdir obj

obj/main.o: src/main.c
	gcc -o obj/main.o -c src/main.c $(CFLAGS) 

obj/events.o: src/events.c lib/events.h
	gcc -o obj/events.o -c src/events.c $(CFLAGS)

obj/render.o: src/render.c lib/render.h
	gcc -o obj/render.o -c src/render.c $(CFLAGS) 

obj/collisions.o: src/collisions.c lib/collisions.h
	gcc -o obj/collisions.o -c src/collisions.c $(CFLAGS)

obj/data.o: src/data.c lib/data.h
	gcc -o obj/data.o -c src/data.c $(CFLAGS)

obj/audio.o: src/audio.c lib/audio.h
	gcc -o obj/audio.o -c src/audio.c $(CFLAGS)

clean:
	rm -rf obj
	rm bin/breakout
