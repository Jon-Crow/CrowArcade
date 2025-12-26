
SRC_DIR   = src
BUILD_DIR = build

GCC_OPTS = -g -O0 -Wall -Wextra -Wpedantic
LIBS     = -lSDL2_image

compile:
	gcc $(GCC_OPTS) \
	    $(SRC_DIR)/*.c \
			-o $(BUILD_DIR)/sdl_test \
			`sdl2-config --cflags --libs` \
			$(LIBS)
