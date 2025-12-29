/**
*@file cgl_spritesheet.h
*@brief Definition of a structure that can interpret a texture as a grid and provide each cell as a texture region.
*
*@author Jonathan Crow
*@date 2025
*/

#ifndef CGL_SPRITESHEET_H
#define CGL_SPRITESHEET_H

#include <SDL2/SDL.h>

#include "cgl_texture.h"

typedef struct CGL_SpriteSheet CGL_SpriteSheet;

/**
*@brief Creates a new spritesheet
*
*@return A pointer to a CGL_SpriteSheet instance, or NULL if there was an error
*/
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

void CGL_SpriteSheetToString(CGL_SpriteSheet* sheet, char *str, size_t strLen);

void CGL_DestroySpriteSheet(CGL_SpriteSheet *sheet);

#endif