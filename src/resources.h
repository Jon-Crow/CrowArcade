
#ifndef RESOURCES_H
#define RESOURCES_H

#include <stdbool.h>

#include "cgl/cgl.h"

#define TEXTURE_COUNT (4) /**< Total number of textures to be globally allocated */

#define TEXTURE_SPLASH_SCREEN  (0) /**< Index of splash screen texture */
#define TEXTURE_PAC_MAN_FONT   (1) /**< Index of pac man font texture */
#define TEXTURE_PAC_MAN_GHOSTS (2) /**< Index of pac man ghosts texture */
#define TEXTURE_FROGGER_FROG   (3) /**< Index of frogger frog texture */

#define TEXTURE_SPLASH_SCREEN_PATH  ("res/texture/stay_storm.png") /**< Path of splash screen texture */
#define TEXTURE_PAC_MAN_FONT_PATH   ("res/texture/pac_font.png")   /**< Path of pac man font texture */
#define TEXTURE_PAC_MAN_GHOSTS_PATH ("res/texture/pac_ghosts.png") /**< Path of pac man ghosts texture */
#define TEXTURE_FROGGER_FROG_PATH   ("res/texture/frog.png")       /**< Path of frogger frog texture */

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
