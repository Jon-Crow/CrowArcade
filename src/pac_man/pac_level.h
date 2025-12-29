
#ifndef PAC_LEVEL_H
#define PAC_LEVEL_H

#include <stdbool.h>

#include "../cgl/cgl.h"

#define PAC_MAN_MAZE_WIDTH  (28)
#define PAC_MAN_MAZE_HEIGHT (31)
#define PAC_MAN_MAZE_SIZE   (868)

#define PAC_MAN_JSON_KEY_TX_COL      ("tx_col")
#define PAC_MAN_JSON_KEY_TX_ROW      ("tx_row")
#define PAC_MAN_JSON_KEY_DOT_COLOR   ("dot_color")
#define PAC_MAN_JSON_KEY_DOT_COLOR_R ("r")
#define PAC_MAN_JSON_KEY_DOT_COLOR_G ("g")
#define PAC_MAN_JSON_KEY_DOT_COLOR_B ("b")
#define PAC_MAN_JSON_KEY_DOT_COLOR_A ("a")
#define PAC_MAN_JSON_KEY_MAZE        ("maze")

typedef enum {
  EMPTY     = 0,
  WALL      = 1,
  DOT       = 2,
  SUPER_DOT = 3
} PacManMazeCellType;

typedef struct PacManLevel PacManLevel;

PacManLevel* CreatePacManLevel(int lvlCol, int lvlRow, const SDL_Color *dotColor);

PacManLevel* LoadPacManLevel(const char *jsonPath);

void PacManLevelRender(CGL_Context *ctx, PacManLevel *lvl, int x, int y);

void DestroyPacManLevel(PacManLevel* level);

#endif
