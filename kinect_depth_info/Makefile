depthinfo: depthinfo.c
	gcc depthinfo.c -Wall -W -pedantic -lfreenect_sync -I/usr/local/include/libfreenect `pkg-config sdl --libs --cflags` -o depthinfo -lGL
depthview: depthview.c
	gcc -Wall -W -pedantic depthview.c `pkg-config sdl --libs --cflags` -lGL -o depthview
clean:
	rm -f depthinfo depthview *~
