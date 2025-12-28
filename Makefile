
SRC_DIR   = ./src
CGL_DIR   = ./src/cgl
BUILD_DIR = ./build

EXECUTABLE = sdl_test

BUILD_TYPE ?= debug

ifeq ($(BUILD_TYPE), debug)
	GCC_OPTS := -g -O0 -fno-omit-frame-pointer -Wall -Wextra -Wpedantic
else
	GCC_OPTS := -O2 -DNDEBUG -Wall
endif

LIBS = -lSDL2_image

VALGRIND_OPTS = --leak-check=full --track-origins=yes --log-file=valgrind.log
CPPCHECK_OPTS = --std=c11 --check-level=exhaustive --template="{file}:{line}: {severity}: {message}" --output-file=cppcheck.log

all: clean compile

clean:
	rm -rf $(BUILD_DIR)

compile:
	mkdir -p $(BUILD_DIR)
	gcc $(GCC_OPTS) \
			$(CGL_DIR)/*.c \
	    $(SRC_DIR)/*.c \
			-o $(BUILD_DIR)/$(EXECUTABLE) \
			`sdl2-config --cflags --libs` \
			$(LIBS)

run:
	$(BUILD_DIR)/$(EXECUTABLE)

run_valgrind:
	valgrind $(VALGRIND_OPTS) $(BUILD_DIR)/$(EXECUTABLE)

run_gdb:
	gdb $(BUILD_DIR)/$(EXECUTABLE)

cppcheck:
	cppcheck $(CPPCHECK_OPTS) $(SRC_DIR)

clang_tidy:
	clang-tidy $(SRC_DIR)/*.c $(CGL_DIR)/*.c -- -std=c11 -Wall -Wextra `sdl2-config --cflags`

docs:
	doxygen Doxyfile
