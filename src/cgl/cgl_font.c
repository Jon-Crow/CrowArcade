/**
*@file cgl_font.c
*@brief Definition of structure that interprets a spritesheet as a font
*
*@author Jonathan Crow
*@date 2025
*/

#include "cgl_font.h"

#include <string.h>

#define CGL_GLYPH_COUNT (128)

typedef struct CGL_Glyph CGL_Glyph;

struct CGL_Glyph {
  int x;
  int y;
};

struct CGL_Font {
  CGL_SpriteSheet *sheet;
  CGL_Glyph glyphs[CGL_GLYPH_COUNT];
};

CGL_Font* CGL_CreateFont(CGL_SpriteSheet *sheet, const char *chars)
{
  if(sheet == NULL)
    return NULL;

  CGL_Font *font = (CGL_Font*)malloc(sizeof(CGL_Font));
  if(font == NULL)
    return NULL;

  font->sheet = sheet;
  for(int i = 0; i < CGL_GLYPH_COUNT; i++)
  {
    font->glyphs[i] = (CGL_Glyph){
      .x = 0,
      .y = 0
    };
  }

  int cols,
      rows;
  CGL_SpriteSheetGetGridSize(sheet, &rows, &cols);

  int len = strlen(chars);
  int x = 0;
  int y = 0;
  for(int i = 0; i < len; i++)
  {
    font->glyphs[(int)chars[i]] = (CGL_Glyph){
      .x = x,
      .y = y
    };
    x++;
    if(x >= cols)
    {
      x = 0;
      y++;
      if(y >= rows)
        return font;
    }
  }

  return font;
}

CGL_SpriteSheet* CGL_FontGetSpriteSheet(CGL_Font *font)
{
  return font->sheet;
}

void CGL_FontGetGlyph(CGL_Font *font, char ch, CGL_TextureRegion *reg)
{
  if(font == NULL)
    return;
  if(font->sheet == NULL)
    return;

  CGL_Glyph glyph = font->glyphs[(int)ch];
  CGL_SpriteSheetGetSpriteAt(font->sheet, glyph.x, glyph.y, reg);
}

void CGL_DestroyFont(CGL_Font *font)
{
  free(font);
}
