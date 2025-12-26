
#ifndef CGL_SPRITESHEET_H
#define CGL_SPRITESHEET_H

typedef struct CGL_SpriteSheet CGL_SpriteSheet;

CGL_SpriteSheet* CGL_CreateSpriteSheet(CGL_Texture *tx, int spriteW, int spriteH);

int CGL_SpriteSheetGetSpriteWidth(CGL_SpriteSheet *sheet);
int CGL_SpriteSheetGetSpriteHeight(CGL_SpriteSheet *sheet);
void CGL_SpriteSheetGetSpriteSize(CGL_SpriteSheet *sheet, int *spriteW, int *spriteH);

int CGL_SpriteSheetGetRows(CGL_SpriteSheet *sheet);
int CGL_SpriteSheetGetColumns(CGL_SpriteSheet *sheet);
void CGL_SpriteSheetGetGridSize(CGL_SpriteSheet *sheet, int *rows, int *cols);

void CGL_DestroySpriteSheet(CGL_SpriteSheet *sheet);

#endif