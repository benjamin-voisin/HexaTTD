CXX = g++
CXXFLAGS = -Wall -Wextra

RAYLIB_DIR = ./raylib/src/

default: hexattd

hexattd: main.o $(RAYLIB_DIR)libraylib.a
	$(CXX) $(CXXFLAGS) -o $@ $^ $(DEPS) 

raylib: $(RAYLIB_DIR)libraylib.a

$(RAYLIB_DIR)libraylib.a:
	$(MAKE) -C $(RAYLIB_DIR)

clean:
	rm -f *.o
	rm -f hexattd
	$(MAKE) clean -C $(RAYLIB_DIR)
