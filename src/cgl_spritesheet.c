
#include "cgl.h"

struct CGL_SpriteSheet {
  CGL_Texture *tx;
  int spriteW;
  int spriteH;
  int rows;
  int cols;
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

void CGL_DestroySpriteSheet(CGL_SpriteSheet *sheet)
{
  if(sheet != NULL)
    free(sheet);
}
