
#ifndef RESOURCES_H
#define RESOURCES_H

#include <stdbool.h>

#include "cgl/cgl.h"

#define TEXTURE_COUNT (4)

#define TEXTURE_SPLASH_SCREEN (0)
#define TEXTURE_PAC_MAN_FONT (1)
#define TEXTURE_PAC_MAN_GHOSTS (2)
#define TEXTURE_FROGGER_GROG (3)

#define TEXTURE_SPLASH_SCREEN_PATH ("res/texture/stay_storm.png")
#define TEXTURE_PAC_MAN_FONT_PATH ("res/texture/pac_font.png")
#define TEXTURE_PAC_MAN_GHOSTS_PATH ("res/texture/pac_ghosts.png")
#define TEXTURE_FROGGER_GROG_PATH ("res/texture/frog.png")

#define FONT_COUNT (1)

#define FONT_PAC_MAN (0)

#define FONT_PAC_MAN_CHAR_WIDTH (8)
#define FONT_PAC_MAN_CHAR_HEIGHT (8)
#define FONT_PAC_MAN_CHARS ("ABCDEFGHIJKLMNOPQRSTUVWXYZ!/-\"0123456789 ")

bool InitResources(CGL_Context *ctx);

CGL_Texture* ResourcesGetTexture(size_t idx);
CGL_Font* ResourcesGetFont(size_t idx);

void DestroyResources();

#endif
