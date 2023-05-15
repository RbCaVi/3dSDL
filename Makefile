
flags := $(flags) -ggdb -g -Og -gdwarf-4
compileflags := -std=c++20 -Wall -Wpedantic -pedantic-errors -Wextra -isystem/opt/opencv/include
linkflags := -Wl,-rpath=/opt/opencv/lib -Wl,-rpath=/opt/gcc-12.2.0/lib64 -L/opt/opencv/lib

.PHONY: all clean

all: main

main.o: main.c++ mainfuncs.c++ window.h++ shaders.h++ matrix.h++ parseargs.h++ shared.h++
	g++ $(flags) $(compileflags) -c main.c++ -o main.o

shaders.o: shaders.c++ shaders.h++ shared.h++
	g++ $(flags) $(compileflags) -c shaders.c++ -o shaders.o

window.o: window.c++ window.h++ texture.h++ shared.h++
	g++ $(flags) $(compileflags) -c window.c++ -o window.o

matrix.o: matrix.c++ matrix.h++ shared.h++
	g++ $(flags) $(compileflags) -c matrix.c++ -o matrix.o

texture.o: texture.c++ texture.h++ shared.h++
	g++ $(flags) $(compileflags) -c texture.c++ -o texture.o

parseargs.o: parseargs.c++ parseargs.h++ shared.h++
	g++ $(flags) $(compileflags) -c parseargs.c++ -o parseargs.o

main: main.o shaders.o window.o matrix.o texture.o parseargs.o
	g++ $(flags) $(linkflags) main.o shaders.o window.o matrix.o texture.o parseargs.o \
	-lSDL2 -lSDL2_image \
	-lGL -lGLEW \
	-lopencv_core -lopencv_highgui \
	-o $@

clean:
	rm main.o
	rm shaders.o
	rm window.o
	rm matrix.o
	rm main
