NAME = hexattd
CXX = g++ --std=c++20 

BUILD_DIR ?= ./build# Default build directory ./build, but modifiable
SRC_DIR = ./src

VPATH += $(BUILD_DIR)

RAYLIB_SRC_PATH ?= ./raylib/src/#Path to raylib source code
RAYGUI_SRC_PATH ?= ./raygui/src/#Path to raygui source code


# This allows the preprocessor to also generate the dependencies in the *.d files
CPPFLAGS += -MP -MD

# Our compile flags, with etra warning
CXXFLAGS = -Wall -Wextra -I$(RAYLIB_SRC_PATH) -I$(SRC_DIR)
DEBUGFLAGS = -g3 -fsanitize=address -DDEBUG -O0
RELEASEFLAGS = -flto -O3 -DNDEBUG

MODE ?= RELEASE# Default is Release

# Raylib has some warnings that we don’t want to see when compiling in debug
CUSTOM_CFLAGS = -Wno-unused-result -Wno-unused-but-set-variable

# Allow usage of mold linker for faster build time, default to false
USE_MOLD_LINKER ?= FALSE
ifeq ($(USE_MOLD_LINKER), TRUE)
	LDFLAGS += -fuse-ld=mold
endif

ifeq ($(MODE),DEBUG) # Debug mode puts address sanitizer and debug info
	CXXFLAGS += $(DEBUGFLAGS)
	CUSTOM_CFLAGS += $(DEBUGFLAGS) # we add our flags to raylib
else # Release mode compile in -O3 with link time optimization
	CXXFLAGS += $(RELEASEFLAGS)
	CUSTOM_CFLAGS += $(RELEASEFLAGS) # we add our flags to raylib
endif

# This part allows us to print progress of the compilation
ifndef ECHO
# We ask `make` for the amount of tasks that we will do
T := $(shell $(MAKE) $(MAKECMDGOALS) --no-print-directory \
      -nrRf $(firstword $(MAKEFILE_LIST)) \
      ECHO="COUNTTHIS" | grep -c "COUNTTHIS")
# N will containt x n times, with n being the number of completed jobs
N := x
# with each call to c, we add an x to N
C = $(words $N)$(eval N := x $N)
ECHO = echo -e "[$(C)/$(T)]" # -e allows us to print color
endif

default: $(NAME)

# for raylib
RAYLIB_RELEASE_PATH = $(CURDIR)/$(BUILD_DIR)
LIBRAYLIB = $(BUILD_DIR)/libraylib.a
export RAYLIB_RELEASE_PATH

# Here we get all the source files and define our objects and makefiles associated
SOURCES = $(shell find $(SRC_DIR) -name '*.cpp')
TEST_FILE = ./src/test.cpp
OBJECTS = $(addprefix $(BUILD_DIR)/, $(SOURCES:%.cpp=%.o))
MAKEFILES = $(OBJECTS:%.o=%.d)

# We include the autogenerated dependencies
-include $(MAKEFILES)

# The main rules to build our projects, either verbosly or not
$(NAME): $(OBJECTS) $(LIBRAYLIB)
	@$(ECHO) "\033[32mBuilding executable $@ in $(MODE) mode\033[0m"
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: %.cpp $(RAYLIB_SRC_PATH)/raylib.h src/gui/clay.h
	@mkdir -p $(BUILD_DIR)/$(dir $<)
	@$(ECHO) "\033[32mBuilding CXX object $@ in $(MODE) mode\033[0m"
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<

web: $(SOURCES) $(RAYLIB_SRC_PATH)/raylib.h src/gui/clay.h
	@mkdir -p $(BUILD_DIR)
	@$(ECHO) "\033[32mBuilding raylib for WASM\033[0m"
	@$(MAKE) -C $(RAYLIB_SRC_PATH) PLATFORM=PLATFORM_WEB -B
	@$(ECHO) "\033[32mBuilding WASM object $(NAME).wasm \033[0m"
	em++ -o $(NAME).html $(SOURCES) -Wall -std=c++20 -D_DEFAULT_SOURCE -Wno-missing-braces -Wunused-result -O3 -s USE_GLFW=3 -s USE_PTHREADS=1 -s ASSERTIONS -s ASYNCIFY -s FORCE_FILESYSTEM=1 --shell-file $(RAYLIB_SRC_PATH)shell.html $(LIBRAYLIB) -DPLATFORM_WEB -s 'EXPORTED_FUNCTIONS=["_free","_malloc","_main"]' -s EXPORTED_RUNTIME_METHODS=ccall -I$(RAYLIB_SRC_PATH) -I$(RAYGUI_SRC_PATH) -I$(SRC_DIR)
	@$(RM) $(LIBRAYLIB)

$(LIBRAYLIB): $(RAYLIB_SRC_PATH)/raylib.h
	@mkdir -p $(BUILD_DIR)
	@$(ECHO) "\033[32mBuilding raylib static lib in $(MODE) mode, this job is longer than others...\033[0m"
	@$(MAKE) -C $(RAYLIB_SRC_PATH) CUSTOM_CFLAGS="$(CUSTOM_CFLAGS)"

test: $(BUILD_DIR)/test
	@./$^

$(BUILD_DIR)/test: $(filter-out $(BUILD_DIR)/./src/main.o,$(OBJECTS)) $(LIBRAYLIB)
	@$(ECHO) "\033[32mBuilding CXX object $(BUILD_DIR)/test.o in $(MODE) mode\033[0m"
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -DTEST -c -o $(BUILD_DIR)/src/test.o $(TEST_FILE)
	@$(ECHO) "\033[32mBuilding unit test executable\033[0m"
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ || echo -e "\n\033[31mFor testing, you should build this project in DEBUG mode.\033[0m\n"
	@$(RM) $(BUILD_DIR)/src/test.o

clean:
	$(RM) $(NAME)
	$(RM) $(NAME).js
	$(RM) $(NAME).wasm
	$(RM) $(NAME).html
	$(RM) $(OBJECTS)
	$(RM) $(MAKEFILES)
	$(RM) $(LIBRAYLIB)
	$(RM) $(BUILD_DIR)/test
	$(RM) $(BUILD_DIR)/src/test.o
	$(RM) $(BUILD_DIR)/src/test.d
	$(RM) $(BUILD_DIR)/test.d
	$(RM) --dir $(shell find $(BUILD_DIR) -type d | sort --reverse)

clean_raylib:
	$(MAKE) clean -C $(RAYLIB_SRC_PATH)

cleanall : clean clean_raylib

.PHONY: clean cleanall clean_raylib test
