
SRC_DIR   = src
CGL_DIR   = src/cgl
BUILD_DIR = build

GCC_OPTS = -g -O0 -fno-omit-frame-pointer -Wall -Wextra -Wpedantic
LIBS     = -lSDL2_image

compile:
	gcc $(GCC_OPTS) \
			$(CGL_DIR)/*.c \
	    $(SRC_DIR)/*.c \
			-o $(BUILD_DIR)/sdl_test \
			`sdl2-config --cflags --libs` \
			$(LIBS)
