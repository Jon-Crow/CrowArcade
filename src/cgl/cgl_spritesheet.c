/**
*@file cgl_spritesheet.c
*@brief Definition of a structure that can interpret a texture as a grid and provide each cell as a texture region.
*
*@author Jonathan Crow
*@date 2025
*/

#include "cgl_spritesheet.h"

/**
*@brief Structure that references a texture and can divide it into a grid
*/
struct CGL_SpriteSheet {
  CGL_Texture *tx; /**< Texture to be divided into a grid */
  int spriteW;     /**< Width of each sprite */
  int spriteH;     /**< Height of each sprite */
  int rows;        /**< Number of rows, based on the texture's height and the given sprite height */
  int cols;        /**< Number of columns, based on the texture's width and the given sprite width */
};

CGL_SpriteSheet* CGL_CreateSpriteSheet(CGL_Texture *tx, int spriteW, int spriteH)
{
  if(tx == NULL)
    return NULL;

  CGL_SpriteSheet *sheet = (CGL_SpriteSheet*)malloc(sizeof(CGL_SpriteSheet));
  if(sheet == NULL)
    return NULL;

  sheet->tx = tx;
  sheet->spriteW = spriteW;
  sheet->spriteH = spriteH;
  sheet->rows = CGL_TextureGetHeight(tx)/spriteH;
  sheet->cols = CGL_TextureGetWidth(tx)/spriteW;

  return sheet;
}

CGL_Texture* CGL_SpriteSheetGetTexture(CGL_SpriteSheet *sheet)
{
  return sheet->tx;
}

SDL_Texture* CGL_SpriteSheetGetImage(CGL_SpriteSheet *sheet)
{
  if(sheet->tx == NULL)
    return NULL;
  return CGL_TextureGetImage(sheet->tx);
}

int CGL_SpriteSheetGetSpriteWidth(CGL_SpriteSheet *sheet)
{
  return sheet->spriteW;
}

int CGL_SpriteSheetGetSpriteHeight(CGL_SpriteSheet *sheet)
{
  return sheet->spriteH;
}

void CGL_SpriteSheetGetSpriteSize(CGL_SpriteSheet *sheet, int *spriteW, int *spriteH)
{
  *spriteW = sheet->spriteW;
  *spriteH = sheet->spriteH;
}

int CGL_SpriteSheetGetRows(CGL_SpriteSheet *sheet)
{
  return sheet->rows;
}

int CGL_SpriteSheetGetColumns(CGL_SpriteSheet *sheet)
{
  return sheet->cols;
}

void CGL_SpriteSheetGetGridSize(CGL_SpriteSheet *sheet, int *rows, int *cols)
{
  *rows = sheet->rows;
  *cols = sheet->cols;
}

void CGL_SpriteSheetGetRectAt(CGL_SpriteSheet *sheet, int col, int row, SDL_Rect *rect)
{
  rect->x = sheet->spriteW * col;
  rect->y = sheet->spriteH * row;
  rect->w = sheet->spriteW;
  rect->h = sheet->spriteH;
}

SDL_Rect CGL_SpriteSheetCreateRectAt(CGL_SpriteSheet *sheet, int col, int row)
{
  SDL_Rect rect;
  CGL_SpriteSheetGetRectAt(sheet, col, row, &rect);
  return rect;
}

void CGL_SpriteSheetGetSpriteAt(CGL_SpriteSheet *sheet, int col, int row, CGL_TextureRegion *reg)
{
  SDL_Rect rect = CGL_SpriteSheetCreateRectAt(sheet, col, row);
  CGL_TextureRegionSetTexture(reg, sheet->tx);
  CGL_TextureRegionSetRect(reg, &rect);
}

void CGL_SpriteSheetGetSpriteAtIndex(CGL_SpriteSheet *sheet, int idx, CGL_TextureRegion *reg)
{
  int col = idx % sheet->cols;
  int row = idx / sheet->cols;
  CGL_SpriteSheetGetSpriteAt(sheet, col, row, reg);
}

CGL_TextureRegion* CGL_SpriteSheetCreateSpriteAt(CGL_SpriteSheet *sheet, int col, int row)
{
  SDL_Rect rect = CGL_SpriteSheetCreateRectAt(sheet, col, row);
  return CGL_CreateTextureRegion(sheet->tx, &rect);
}

void CGL_SpriteSheetToString(CGL_SpriteSheet* sheet, char *str, size_t strLen)
{
  snprintf(str, strLen, "CGL_SpriteSheet{tx=\"%s\",spriteW=%d, spriteH=%d, cols=%d, rows=%d}", 
    CGL_TextureGetPath(sheet->tx),
    sheet->spriteW, 
    sheet->spriteH, 
    sheet->cols, 
    sheet->rows
  );
}

void CGL_DestroySpriteSheet(CGL_SpriteSheet *sheet)
{
  if(sheet != NULL)
    free(sheet);
}
