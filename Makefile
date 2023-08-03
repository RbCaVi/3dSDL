
#.SILENT:

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

compilecmd = g++ $(flags) $(compileflags) $(CXXFLAGS)
linkcmd = g++ $(flags) $(linkflags) $(LDFLAGS)

.PHONY: all clean packassets unpackassets checkflags

all: objmain randmain chunkobjsmain

.flags:
	if test \\\! -f .flags; touch .flags; fi

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

assets.o: file.h++

randmain.o: randmain.c++ random.h++ chunk.h++

objmain.o: objmainfuncs.c++ window.h++ shaders.h++ matrix.h++ parseargs.h++ obj.h++ assets.h++

packedassets.o: packedassets.S

# merge is ld -r a.o b.o -o c.o

getpackedassets.o: packedassets.c++|checkflags
	$(compilecmd) -c packedassets.c++ -o getpackedassets.o

packedassets.S: packassets.sh assets/cat.obj assets/objshader.frag assets/objshader.vert
	bash packassets.sh packedassets.S cat.obj objshader.frag objshader.vert

packassets: packedassets.o getpackedassets.o
	ld -r getpackedassets.o packedassets.o -o assets.o

unpackassets:
	rm assets.o

randmain: randmain.o random.o chunk.o|checkflags
	$(linkcmd) $^ -o $@

objmain: objmain.o shaders.o window.o matrix.o texture.o parseargs.o obj.o file.o assets.o|checkflags
	$(linkcmd) $^ \
	-lSDL2 \
	-lGL -lGLEW \
	$(opencv-libs) \
	-o $@

chunkobjsmain: chunkobjsmain.o shaders.o window.o matrix.o texture.o parseargs.o objs.o file.o assets.o chunk.o random.o|checkflags
	$(linkcmd) $^ \
	-lSDL2 \
	-lGL -lGLEW \
	$(opencv-libs) \
	-o $@

clean:
	rm -f *.o
	rm -f objmain
	rm -f chunkobjsmain
	rm -f randmain
	rm -f packedassets.S

debug-%:
	make $* DEBUG=1

profile-%:
	make $* PROFILE=1

opencv-%:
	make $* OPENCV=1
