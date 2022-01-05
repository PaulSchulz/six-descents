.phony: all clean

all: display-events version version2

display-events: display-events.c
	gcc -o $@ $< -lsqlite3

version: version.c
	gcc -o version version.c -lsqlite3

version2: version2.c
	gcc -o version2 version2.c -lsqlite3

clean:
	-rm display-events version version2
