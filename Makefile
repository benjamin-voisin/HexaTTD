CXX = g++
DEBUGFLAGS = -g3 -fsanitize=address -fsanitize=leak
CXXFLAGS = -Wall -Wextra -O2 $(DEBUGFLAGS)

RAYLIB_DIR = ./raylib/src/

default: hexattd

hexattd: src/main.o src/hex.o src/grid.o $(RAYLIB_DIR)libraylib.a
	$(CXX) $(CXXFLAGS) -o $@ $^ $(DEPS) 

raylib: $(RAYLIB_DIR)libraylib.a

$(RAYLIB_DIR)libraylib.a:
	$(MAKE) -C $(RAYLIB_DIR)

clean: clean_hex clean_raylib

clean_hex:
	$(MAKE) clean -C ./src
	rm -f ./hexattd

clean_raylib:
	$(MAKE) clean -C $(RAYLIB_DIR)
