
#ifndef PAC_LEVEL_H
#define PAC_LEVEL_H

#include <stdbool.h>

#include "../cgl/cgl.h"

#define PAC_MAN_MAZE_WIDTH  (28)
#define PAC_MAN_MAZE_HEIGHT (31)
#define PAC_MAN_MAZE_SIZE   (868)

typedef enum PacManMazeCellType {
  EMPTY,
  WALL,
  DOT,
  SUPER_DOT
};

typedef struct PacManLevel PacManLevel;

PacManLevel* CreatePacManLevel();

void DestroyPacManLevel(PacManLevel* level);

#endif
