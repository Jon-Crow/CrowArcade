
#ifndef RESOURCES_H
#define RESOURCES_H

#include <stdbool.h>

#include "cJSON.h"

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

#define SPRITE_SHEET_COUNT (6) /**< */

#define SPRITE_SHEET_SPLASH_SCREEN  (0) /**< */
#define SPRITE_SHEET_PAC_MAN_FONT   (1) /**< */
#define SPRITE_SHEET_PAC_MAN_PLAYER (2) /**< */
#define SPRITE_SHEET_PAC_MAN_GHOSTS (3) /**< */
#define SPRITE_SHEET_PAC_MAN_LEVELS (4) /**< */
#define SPRITE_SHEET_FROGGER_FROG   (5) /**< */

#define SPRITE_SHEET_SPLASH_SCREEN_SPRITE_WIDTH   (224) /**< */
#define SPRITE_SHEET_SPLASH_SCREEN_SPRITE_HEIGHT  (288) /**< */
#define SPRITE_SHEET_PAC_MAN_FONT_SPRITE_WIDTH    (8)   /**< */
#define SPRITE_SHEET_PAC_MAN_FONT_SPRITE_HEIGHT   (8)   /**< */
#define SPRITE_SHEET_PAC_MAN_PLAYER_SPRITE_WIDTH  (16)  /**< */
#define SPRITE_SHEET_PAC_MAN_PLAYER_SPRITE_HEIGHT (16)  /**< */
#define SPRITE_SHEET_PAC_MAN_GHOSTS_SPRITE_WIDTH  (16)  /**< */
#define SPRITE_SHEET_PAC_MAN_GHOSTS_SPRITE_HEIGHT (16)  /**< */
#define SPRITE_SHEET_PAC_MAN_LEVELS_SPRITE_WIDTH  (224) /**< */
#define SPRITE_SHEET_PAC_MAN_LEVELS_SPRITE_HEIGHT (248) /**< */
#define SPRITE_SHEET_FROGGER_FROG_SPRITE_WIDTH    (16)  /**< */
#define SPRITE_SHEET_FROGGER_FROG_SPRITE_HEIGHT   (16)  /**< */

#define FONT_COUNT (1) /**< Total number of fonts to be globally allocated */

#define FONT_PAC_MAN (0) /**< Index of pac man font */

#define FONT_PAC_MAN_CHARS ("ABCDEFGHIJKLMNOPQRSTUVWXYZ!/-\"0123456789 ") /**< Pac man font characters */

#define SOUND_COUNT (1)

#define SOUND_PAC_MAN_START (0)

#define SOUND_PAC_MAN_START_PATH ("res/sound/pac_man/ms_start.wav")

bool InitResources(CGL_Context *ctx);

CGL_Texture* ResourcesGetTexture(size_t idx);
CGL_SpriteSheet* ResourcesGetSpriteSheet(size_t idx);
CGL_Font* ResourcesGetFont(size_t idx);
CGL_Sound* ResourcesGetSound(size_t idx);

cJSON* ResourcesParseJsonFile(const char *path);

void DestroyResources();

#endif
