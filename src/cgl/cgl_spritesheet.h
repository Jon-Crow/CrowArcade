
#ifndef CGL_SPRITESHEET_H
#define CGL_SPRITESHEET_H

#include <SDL2/SDL.h>

#include "cgl_texture.h"
#include "cgl_spritesheet.h"

typedef struct CGL_SpriteSheet CGL_SpriteSheet;

CGL_SpriteSheet* CGL_CreateSpriteSheet(CGL_Texture *tx, int spriteW, int spriteH);

CGL_Texture* CGL_SpriteSheetGetTexture(CGL_SpriteSheet *sheet);
SDL_Texture* CGL_SpriteSheetGetImage(CGL_SpriteSheet *sheet);

int CGL_SpriteSheetGetSpriteWidth(CGL_SpriteSheet *sheet);
int CGL_SpriteSheetGetSpriteHeight(CGL_SpriteSheet *sheet);
void CGL_SpriteSheetGetSpriteSize(CGL_SpriteSheet *sheet, int *spriteW, int *spriteH);

int CGL_SpriteSheetGetRows(CGL_SpriteSheet *sheet);
int CGL_SpriteSheetGetColumns(CGL_SpriteSheet *sheet);
void CGL_SpriteSheetGetGridSize(CGL_SpriteSheet *sheet, int *rows, int *cols);

void CGL_SpriteSheetGetRectAt(CGL_SpriteSheet *sheet, int col, int row, SDL_Rect *rect);
SDL_Rect CGL_SpriteSheetCreateRectAt(CGL_SpriteSheet *sheet, int col, int row);
void CGL_SpriteSheetGetSpriteAt(CGL_SpriteSheet *sheet, int col, int row, CGL_TextureRegion *reg);
void CGL_SpriteSheetGetSpriteAtIndex(CGL_SpriteSheet *sheet, int idx, CGL_TextureRegion *reg);
CGL_TextureRegion* CGL_SpriteSheetCreateSpriteAt(CGL_SpriteSheet *sheet, int col, int row);

void CGL_DestroySpriteSheet(CGL_SpriteSheet *sheet);

#endif