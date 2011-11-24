draw: draw.c
	gcc draw.c -Wall -W -pedantic -lfreenect_sync -I/usr/local/include/libfreenect `pkg-config sdl --libs --cflags` -o draw -lGL
clean:
	rm -f draw *~
