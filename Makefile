
flags := -ggdb -g -Og -gdwarf-4
compileflags := -std=c++20 -Wall -Wpedantic
linkflags := -Wl,-rpath=/opt/gcc-12.2.0/lib64

.PHONY: all clean

all: main

main.o: main.c++ window.h shaders.h shared.h
	g++ $(flags) $(compileflags) -c main.c++ -o main.o

shaders.o: shaders.c++ shaders.h shared.h
	g++ $(flags) $(compileflags) -c shaders.c++ -o shaders.o

window.o: window.c++ window.h shared.h
	g++ $(flags) $(compileflags) -c window.c++ -o window.o

matrix.o: matrix.c++ matrix.h shared.h
	g++ $(flags) $(compileflags) -c matrix.c++ -o matrix.o

main: main.o shaders.o window.o matrix.o
	g++ $(flags) $(linkflags) main.o shaders.o window.o matrix.o -lSDL2 -lGL -lGLEW -o $@

clean:
	rm main.o
	rm shaders.o
	rm window.o
	rm main
