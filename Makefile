
flags := $(flags) -Wall -ggdb -g -Og -gdwarf-4
compileflags := -std=c++20 -Wall -Wpedantic -pedantic-errors -Wextra -isystem/opt/opencv/include
linkflags := -Wl,-rpath=/opt/opencv/lib -Wl,-rpath=/opt/gcc-12.2.0/lib64 -L/opt/opencv/lib

.PHONY: all clean

all: objmain

file.o: file.c++ file.h++ shared.h++
	g++ $(flags) $(compileflags) -c file.c++ -o file.o

parseargs.o: parseargs.c++ parseargs.h++ shared.h++
	g++ $(flags) $(compileflags) -c parseargs.c++ -o parseargs.o

shaders.o: shaders.c++ shaders.h++ shared.h++
	g++ $(flags) $(compileflags) -c shaders.c++ -o shaders.o

matrix.o: matrix.c++ matrix.h++ shared.h++
	g++ $(flags) $(compileflags) -c matrix.c++ -o matrix.o

texture.o: texture.c++ texture.h++ shared.h++
	g++ $(flags) $(compileflags) -c texture.c++ -o texture.o

window.o: window.c++ window.h++ texture.h++ shaders.h++ shared.h++
	g++ $(flags) $(compileflags) -c window.c++ -o window.o

# merge is ld -r a.o b.o -o c.o

obj.o: obj.c++ obj.h++ file.h++ shared.h++
	g++ $(flags) $(compileflags) -c obj.c++ -o obj.o

assets.o: assets.c++ assets.h++ file.h++ shared.h++
	g++ $(flags) $(compileflags) -c assets.c++ -o assets.o

packedassets.S: packassets.sh assets/cat.obj assets/objshader.frag assets/objshader.vert
	bash packassets.sh packedassets.S cat.obj objshader.frag objshader.vert

packedassets.o: packedassets.S
	g++ $(flags) $(compileflags) -c packedassets.S -o packedassets.o

packassets: packedassets.o
	g++ $(flags) $(compileflags) -c packedassets.c++ -o getpackedassets.o
	ld -r getpackedassets.o packedassets.o -o assets.o

unpackassets:
	rm assets.o

objmain.o: objmain.c++ objmainfuncs.c++ objmain.h++ window.h++ shaders.h++ matrix.h++ parseargs.h++ obj.h++ assets.h++ shared.h++
	g++ $(flags) $(compileflags) -c objmain.c++ -o objmain.o

objmain: objmain.o shaders.o window.o matrix.o texture.o parseargs.o obj.o file.o assets.o
	g++ $(flags) $(linkflags) objmain.o shaders.o window.o matrix.o texture.o parseargs.o file.o obj.o assets.o \
	-lSDL2 -lSDL2_image \
	-lGL -lGLEW \
	-lopencv_core -lopencv_highgui \
	-o $@

clean:
	rm -f file.o
	rm -f shaders.o
	rm -f texture.o
	rm -f matrix.o
	rm -f window.o
	rm -f parseargs.o
	rm -f obj.o
	rm -f objmain.o
	rm -f objmain
	rm -f assets.o
	rm -f getpackedassets.o
	rm -f packedassets.o
	rm -f packedassets.S
