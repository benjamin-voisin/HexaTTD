NAME = hexattd
CXX = g++ --std=c++20 

BUILD_DIR ?= ./build# Default build directory ./build, but modifiable

VPATH += $(BUILD_DIR)

RAYLIB_SRC_PATH ?= ./raylib/src/#Path to raylib source code
RAYGUI_SRC_PATH ?= ./raygui/src/#Path to raygui source code


# This allows the preprocessor to also generate the dependencies in the *.d files
CPPFLAGS += -MP -MD

# Our compile flags, with etra warning
CXXFLAGS = -Wall -Wextra -I$(RAYLIB_SRC_PATH) -I$(RAYGUI_SRC_PATH)
DEBUGFLAGS = -g3 -fsanitize=address
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
export CUSTOM_CFLAGS
export RAYLIB_RELEASE_PATH

# Here we get all the source files and define our objects and makefiles associated
SOURCES = $(shell find . -name '*.cpp')
OBJECTS = $(addprefix $(BUILD_DIR)/, $(SOURCES:%.cpp=%.o))
MAKEFILES = $(OBJECTS:%.o=%.d)

# We include the autogenerated dependencies
-include $(MAKEFILES)

# The main rules to build our projects, either verbosly or not
# @$(ECHO) "\033[32m$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^\033[0m"
$(NAME): $(OBJECTS) $(LIBRAYLIB)
	@$(ECHO) "\033[32mBuilding executable $@ in $(MODE) mode\033[0m"
	@$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

# @$(ECHO) "\033[32m$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<\033[0m"
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(BUILD_DIR)/$(dir $<)
	@$(ECHO) "\033[32mBuilding CXX object $@ in $(MODE) mode\033[0m"
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<

$(LIBRAYLIB):
	@mkdir -p $(BUILD_DIR)
	@$(ECHO) "\033[32mBuilding raylib static lib in $(MODE) mode, this job is longer than others...\033[0m"
	@$(MAKE) -C $(RAYLIB_SRC_PATH)

clean: clean_hex

cleanall : clean clean_raylib

clean_hex:
	$(RM) $(NAME)
	$(RM) $(OBJECTS)
	$(RM) $(MAKEFILES)
	$(RM) $(LIBRAYLIB)
	$(RM) --dir $(shell find $(BUILD_DIR) -type d | sort --reverse)

clean_raylib:
	$(MAKE) clean -C $(RAYLIB_SRC_PATH)

.PHONY: clean cleanall clean_hex clean_raylib
