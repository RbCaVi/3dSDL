
flags := -Wall
compileflags := -std=c++20 -Wall -Wpedantic -pedantic-errors -Wextra -isystem/opt/opencv/include
linkflags := -Wl,-rpath=/opt/opencv/lib -Wl,-rpath=/opt/gcc-12.2.0/lib64 -L/opt/opencv/lib

ifdef DEBUG
	flags := $(flags) -ggdb -g -Og -gdwarf-4 -DDEBUG
endif

ifdef PROFILE
	flags := $(flags) -pg
endif

.PHONY: all clean packassets unpackassets

all: objmain randmain chunkmain

file.o: file.c++ file.h++ shared.h++
	g++ $(flags) $(compileflags) $(CFLAGS) -c file.c++ -o file.o

random.o: random.c++ random.h++ shared.h++
	g++ $(flags) $(compileflags) $(CFLAGS) -c random.c++ -o random.o

parseargs.o: parseargs.c++ parseargs.h++ shared.h++
	g++ $(flags) $(compileflags) $(CFLAGS) -c parseargs.c++ -o parseargs.o

shaders.o: shaders.c++ shaders.h++ shared.h++
	g++ $(flags) $(compileflags) $(CFLAGS) -c shaders.c++ -o shaders.o

matrix.o: matrix.c++ matrix.h++ shared.h++
	g++ $(flags) $(compileflags) $(CFLAGS) -c matrix.c++ -o matrix.o

texture.o: texture.c++ texture.h++ shared.h++
	g++ $(flags) $(compileflags) $(CFLAGS) -c texture.c++ -o texture.o

window.o: window.c++ window.h++ texture.h++ shaders.h++ shared.h++
	g++ $(flags) $(compileflags) $(CFLAGS) -c window.c++ -o window.o

chunk.o: chunk.c++ chunk.h++ shared.h++
	g++ $(flags) $(compileflags) $(CFLAGS) -c chunk.c++ -o chunk.o

# merge is ld -r a.o b.o -o c.o

obj.o: obj.c++ obj.h++ file.h++ shared.h++
	g++ $(flags) $(compileflags) $(CFLAGS) -c obj.c++ -o obj.o

assets.o: assets.c++ assets.h++ file.h++ shared.h++
	g++ $(flags) $(compileflags) $(CFLAGS) -c assets.c++ -o assets.o

packedassets.S: packassets.sh assets/cat.obj assets/objshader.frag assets/objshader.vert
	bash packassets.sh packedassets.S cat.obj objshader.frag objshader.vert

packedassets.o: packedassets.S
	g++ $(flags) $(compileflags) $(CFLAGS) -c packedassets.S -o packedassets.o

packassets: packedassets.o
	g++ $(flags) $(compileflags) $(CFLAGS) -c packedassets.c++ -o getpackedassets.o
	ld -r getpackedassets.o packedassets.o -o assets.o

unpackassets:
	rm assets.o

randmain.o: randmain.c++ random.h++ shared.h++
	g++ $(flags) $(compileflags) $(CFLAGS) -c randmain.c++ -o randmain.o

randmain: randmain.o random.o chunk.o
	g++ $(flags) $(linkflags) $(CFLAGS) randmain.o random.o chunk.o \
	-o $@

objmain.o: objmain.c++ objmainfuncs.c++ objmain.h++ window.h++ shaders.h++ matrix.h++ parseargs.h++ obj.h++ assets.h++ shared.h++
	g++ $(flags) $(compileflags) $(CFLAGS) -c objmain.c++ -o objmain.o

objmain: objmain.o shaders.o window.o matrix.o texture.o parseargs.o obj.o file.o assets.o
	g++ $(flags) $(linkflags) $(CFLAGS) objmain.o shaders.o window.o matrix.o texture.o parseargs.o file.o obj.o assets.o \
	-lSDL2 -lSDL2_image \
	-lGL -lGLEW \
	-lopencv_core -lopencv_highgui \
	-o $@

chunkmain.o: chunkmain.c++ chunkmainfuncs.c++ chunkmain.h++ window.h++ shaders.h++ matrix.h++ parseargs.h++ obj.h++ assets.h++ shared.h++
	g++ $(flags) $(compileflags) $(CFLAGS) -c chunkmain.c++ -o chunkmain.o

chunkmain: chunkmain.o shaders.o window.o matrix.o texture.o parseargs.o obj.o file.o assets.o
	g++ $(flags) $(linkflags) $(CFLAGS) chunkmain.o shaders.o window.o matrix.o texture.o parseargs.o file.o obj.o assets.o \
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
	rm -f random.o
	rm -f randmain.o
	rm -f chunk.o
	rm -f chunkmain.o
	rm -f chunkmain
	rm -f assets.o
	rm -f getpackedassets.o
	rm -f packedassets.o
	rm -f packedassets.S

debug-%:
	make $* DEBUG=1

profile-%:
	make $* PROFILE=1