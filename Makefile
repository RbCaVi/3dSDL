
flags := -Wall -Wpedantic -pedantic-errors -Wextra
compileflags := -std=c++20
linkflags := -Wl,-rpath=/opt/gcc-12.2.0/lib64

ifdef DEBUG
	flags := $(flags) -ggdb -g -Og -gdwarf-4 -DDEBUG
endif

ifdef PROFILE
	flags := $(flags) -pg
endif

ifdef OPENCV
	OPENCV_PATH := /opt/opencv
	compileflags := $(compileflags) -DHAS_OPENCV -isystem$(OPENCV_PATH)/include
	linkflags := $(linkflags) -Wl,-rpath=$(OPENCV_PATH)/lib -L$(OPENCV_PATH)/lib
	opencv-libs := -lopencv_core -lopencv_highgui
endif

.PHONY: all clean packassets unpackassets

all: objmain randmain chunkmain

%.o: %.c++ %.h++ shared.h++
	 g++ $(flags) $(compileflags) $(CFLAGS) -c $< -o $@

window.o: texture.h++ shaders.h++

obj.o: file.h++

assets.o: file.h++

randmain.o: randmain.c++ random.h++ chunk.h++

objmain.o: objmainfuncs.c++ window.h++ shaders.h++ matrix.h++ parseargs.h++ obj.h++ assets.h++

chunkmain.o: chunkmainfuncs.c++ window.h++ shaders.h++ matrix.h++ parseargs.h++ obj.h++ assets.h++

packedassets.o: packedassets.S

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
	$(opencv-libs) \
	-o $@

chunkmain: chunkmain.o shaders.o window.o matrix.o texture.o parseargs.o obj.o file.o assets.o chunk.o random.o
	g++ $(flags) $(linkflags) $(CFLAGS) $^ \
	-lSDL2 \
	-lGL -lGLEW \
	$(opencv-libs) \
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

opencv-%:
	make $* OPENCV=1
