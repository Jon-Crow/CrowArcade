
#ifndef RESOURCES_H
#define RESOURCES_H

#include <stdbool.h>

#include "cgl/cgl.h"

#define TEXTURE_COUNT (6) /**< Total number of textures to be globally allocated */

#define TEXTURE_SPLASH_SCREEN  (0) /**< Index of splash screen texture */
#define TEXTURE_PAC_MAN_FONT   (1) /**< Index of pac man font texture */
#define TEXTURE_PAC_MAN_PLAYER (2) /**< Index of ms pac man texture */
#define TEXTURE_PAC_MAN_GHOSTS (3) /**< Index of pac man ghosts texture */
#define TEXTURE_PAC_MAN_LEVELS (4) /**< Index of pac man levels texture */
#define TEXTURE_FROGGER_FROG   (5) /**< Index of frogger frog texture */

#define TEXTURE_SPLASH_SCREEN_PATH  ("res/texture/stay_storm.png")         /**< Path of splash screen texture */
#define TEXTURE_PAC_MAN_FONT_PATH   ("res/texture/pac_man/font.png")       /**< Path of pac man font texture */
#define TEXTURE_PAC_MAN_PLAYER_PATH ("res/texture/pac_man/ms_pac_man.png") /**< Path of ms pac man texture */
#define TEXTURE_PAC_MAN_GHOSTS_PATH ("res/texture/pac_man/ghosts.png")     /**< Path of pac man ghosts texture */
#define TEXTURE_PAC_MAN_LEVELS_PATH ("res/texture/pac_man/levels.png")     /**< Index of pac man levels texture */
#define TEXTURE_FROGGER_FROG_PATH   ("res/texture/frogger/frog.png")       /**< Path of frogger frog texture */

#define FONT_COUNT (1) /**< Total number of fonts to be globally allocated */

#define FONT_PAC_MAN (0) /**< Index of pac man font */

#define FONT_PAC_MAN_CHAR_WIDTH  (8) /**< Pac man font character width */
#define FONT_PAC_MAN_CHAR_HEIGHT (8) /**< Pac man font character height */

#define FONT_PAC_MAN_CHARS ("ABCDEFGHIJKLMNOPQRSTUVWXYZ!/-\"0123456789 ") /**< Pac man font characters */

bool InitResources(CGL_Context *ctx);

CGL_Texture* ResourcesGetTexture(size_t idx);
CGL_Font* ResourcesGetFont(size_t idx);

void DestroyResources();

#endif
