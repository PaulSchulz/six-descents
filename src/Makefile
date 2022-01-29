.PHONY: all clean

TARGETS=display-events-sdl display-events version
all: $(TARGETS)

display-events-sdl: display-events-sdl.c hr_time.c
	gcc -o $@ $^ -lsqlite3 -lSDL2 -lSDL2_ttf

display-events: display-events.c
	gcc -o $@ $< -lsqlite3

version: version.c
	gcc -o version version.c -lsqlite3

clean:
	-rm -f $(TARGETS)
	-rm -f *~
