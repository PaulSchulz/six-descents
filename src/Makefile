.phony: all clean

all: display-events-sdl display-events version version2 sdltest

display-events-sdl: display-events-sdl.c hr_time.c
	gcc -o $@ $^ -lsqlite3 -lSDL2 -lSDL2_ttf

display-events: display-events.c
	gcc -o $@ $< -lsqlite3

version: version.c
	gcc -o version version.c -lsqlite3

version2: version2.c
	gcc -o version2 version2.c -lsqlite3

sdltest: sdltest.c hr_time.c
	gcc -o sdltest hr_time.c sdltest.c -lSDL2 -lSDL2_ttf

clean:
	-rm -f display-events-sdl display-events version version2 sdltest
	-rm -f *~
