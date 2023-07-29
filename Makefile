
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
	g++ $(flags) $(compileflags) $(CFLAGS) -c $< -o $@

random.o: random.c++ random.h++ shared.h++
	g++ $(flags) $(compileflags) $(CFLAGS) -c $< -o $@

parseargs.o: parseargs.c++ parseargs.h++ shared.h++
	g++ $(flags) $(compileflags) $(CFLAGS) -c $< -o $@

shaders.o: shaders.c++ shaders.h++ shared.h++
	g++ $(flags) $(compileflags) $(CFLAGS) -c $< -o $@

matrix.o: matrix.c++ matrix.h++ shared.h++
	g++ $(flags) $(compileflags) $(CFLAGS) -c $< -o $@

texture.o: texture.c++ texture.h++ shared.h++
	g++ $(flags) $(compileflags) $(CFLAGS) -c $< -o $@

window.o: window.c++ window.h++ texture.h++ shaders.h++ shared.h++
	g++ $(flags) $(compileflags) $(CFLAGS) -c $< -o $@

chunk.o: chunk.c++ chunk.h++ shared.h++
	g++ $(flags) $(compileflags) $(CFLAGS) -c $< -o $@

obj.o: obj.c++ obj.h++ file.h++ shared.h++
	g++ $(flags) $(compileflags) $(CFLAGS) -c $< -o $@

assets.o: assets.c++ assets.h++ file.h++ shared.h++
	g++ $(flags) $(compileflags) $(CFLAGS) -c $< -o $@

packedassets.o: packedassets.S
	g++ $(flags) $(compileflags) $(CFLAGS) -c $< -o $@

randmain.o: randmain.c++ random.h++ shared.h++
	g++ $(flags) $(compileflags) $(CFLAGS) -c $< -o $@

objmain.o: objmain.c++ objmainfuncs.c++ objmain.h++ window.h++ shaders.h++ matrix.h++ parseargs.h++ obj.h++ assets.h++ shared.h++
	g++ $(flags) $(compileflags) $(CFLAGS) -c $< -o $@

chunkmain.o: chunkmain.c++ chunkmainfuncs.c++ chunkmain.h++ window.h++ shaders.h++ matrix.h++ parseargs.h++ obj.h++ assets.h++ shared.h++
	g++ $(flags) $(compileflags) $(CFLAGS) -c $< -o $@


# merge is ld -r a.o b.o -o c.o

packedassets.S: packassets.sh assets/cat.obj assets/objshader.frag assets/objshader.vert
	bash packassets.sh packedassets.S cat.obj objshader.frag objshader.vert

packassets: packedassets.o
	g++ $(flags) $(compileflags) $(CFLAGS) -c packedassets.c++ -o getpackedassets.o
	ld -r getpackedassets.o packedassets.o -o assets.o

unpackassets:
	rm assets.o

randmain: randmain.o random.o chunk.o
	g++ $(flags) $(linkflags) $(CFLAGS) $^ -o $@

objmain: objmain.o shaders.o window.o matrix.o texture.o parseargs.o obj.o file.o assets.o
	g++ $(flags) $(linkflags) $(CFLAGS) $^ \
	-lSDL2 \
	-lGL -lGLEW \
	-lopencv_core -lopencv_highgui \
	-o $@

chunkmain: chunkmain.o shaders.o window.o matrix.o texture.o parseargs.o obj.o file.o assets.o
	g++ $(flags) $(linkflags) $(CFLAGS) $^ \
	-lSDL2 \
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
