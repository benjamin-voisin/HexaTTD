CXX = g++
VPATH = ./src
RAYLIB_DIR = ./raylib/src/

CXXBASEFLAGS = -Wall -Wextra
DEBUGFLAGS = -g -fsanitize=address -fsanitize=leak
RELEASEFLAGS = -flto -O3

MODE ?= DEBUG# Default is Debug, any other value will build for release

# Raylib has some warnings that we don’t want to see when compiling in debug
RAYLIB_IGNORE_WARNINGS = -Wno-unused-result -Wno-unused-but-set-variable
ifeq ($(MODE),DEBUG)
	CXXFLAGS = $(CXXBASEFLAGS) $(DEBUGFLAGS)
	CUSTOM_CFLAGS = $(RAYLIB_IGNORE_WARNINGS) $(DEBUGFLAGS)
else
	CXXFLAGS = $(CXXBASEFLAGS) $(RELEASEFLAGS)
	CUSTOM_CFLAGS = $(RELEASEFLAGS)
endif

# for raylib
RAYLIB_RELEASE_PATH = ../../
export CUSTOM_CFLAGS
export RAYLIB_RELEASE_PATH

default: hexattd

hexattd: main.o hex.o grid.o rail.o base.o vector.o display_track.o libraylib.a
	$(CXX) $(CXXFLAGS) -o $@ $^ $(DEPS) 

libraylib.a:
	$(MAKE) -C $(RAYLIB_DIR)

clean: clean_hex clean_raylib

clean_hex:
	rm -f ./hexattd
	rm -f *.o

clean_raylib:
	$(MAKE) clean -C $(RAYLIB_DIR)
