
#.SILENT:

flags := -Wall -Wpedantic -pedantic-errors -Wextra
compileflags := -std=c++2a -I.
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

compilecmd = g++ $(flags) $(compileflags) $(CXXFLAGS)
linkcmd = g++ $(flags) $(linkflags) $(LDFLAGS)

.PHONY: all clean packassets unpackassets checkflags

all: out/objmain out/randmain out/chunkmain

.flags:
	if test \\\! -f .flags; then touch .flags; fi

checkflags: .flags
	if echo "$(compilecmd)" "$(linkcmd)" "$(opencv-libs)"|cmp .flags; then \
		:; \
	else \
		echo "$(compilecmd)" "$(linkcmd)" "$(opencv-libs)">.flags; \
		make $(MAKECMDGOALS); \
	fi

%.o: %.c++ %.h++ shared.h++ .flags|checkflags
	 $(compilecmd) -c $< -o $@

window.o: texture.h++ shaders.h++

obj.o: file.h++

objs.o: file.h++

assets/assets.o: file.h++ out/assets

randmain/randmain.o: randmain/randmain.c++ random.h++ chunk.h++

objmain/objmain.o: objmain/objmainfuncs.c++ window.h++ shaders.h++ matrix.h++ parseargs.h++ obj.h++ assets/assets.h++

chunkmain/chunkmain.o: chunkmain/chunkmainfuncs.c++ chunkmain/chunkmain.h++ assets/assets.h++ parseargs.h++ shaders.h++ window.h++ matrix.h++ random.h++ chunk.h++ objs.h++

assets/packedassets.o: assets/packedassets.S

# merge is ld -r a.o b.o -o c.o

assets/getpackedassets.o: assets/packedassets.c++|checkflags
	$(compilecmd) -c assets/packedassets.c++ -o assets/getpackedassets.o

assets/packedassets.S: assets/packassets.sh assets/assets/cat.obj assets/assets/objshader.frag assets/assets/objshader.vert
	bash assets/packassets.sh assets/packedassets.S assets/cat.obj assets/objshader.frag assets/objshader.vert

packassets: assets/packedassets.o assets/getpackedassets.o
	ld -r assets/getpackedassets.o assets/packedassets.o -o assets/assets.o

unpackassets:
	rm assets/assets.o

out/randmain: randmain/randmain.o random.o chunk.o|checkflags
	$(linkcmd) $^ -o $@

out/objmain: objmain/objmain.o shaders.o window.o matrix.o texture.o parseargs.o obj.o file.o assets/assets.o|checkflags
	$(linkcmd) $^ \
	-lSDL2 \
	-lGL -lGLEW \
	$(opencv-libs) \
	-o $@

out/chunkmain: chunkmain/chunkmain.o shaders.o window.o matrix.o texture.o parseargs.o objs.o file.o assets/assets.o chunk.o random.o|checkflags
	$(linkcmd) $^ \
	-lSDL2 \
	-lGL -lGLEW \
	$(opencv-libs) \
	-o $@

out:
	mkdir -p out

out/assets: out assets/assets/*
	cp -r assets/assets out

clean:
	rm -f *.o
	rm -f */*.o
	rm -f out/objmain
	rm -f out/chunkmain
	rm -f out/randmain
	rm -f assets/packedassets.S

debug-%:
	make $* DEBUG=1

profile-%:
	make $* PROFILE=1

opencv-%:
	make $* OPENCV=1
