/**
*@file cgl_font.c
*@brief Definition of structure that interprets a spritesheet as a font
*
*@author Jonathan Crow
*@date 2025
*/

#include "cgl_font.h"

#include <string.h>

#define CGL_GLYPH_COUNT (128) /**< Maximum necessary size for an array of all possible characters */

typedef struct CGL_Glyph CGL_Glyph;

/**
*@brief Simple coordinate struct to represent the position of a character in a spritesheet
*/
struct CGL_Glyph {
  int x;
  int y;
};

/**
*@brief A container for a spritesheet of characters, and where each character is located
*/
struct CGL_Font {
  CGL_SpriteSheet *sheet;            /**< Pointer to the spritesheet to be referenced */
  CGL_Glyph glyphs[CGL_GLYPH_COUNT];
};

/**
*@var CGL_Font::glyphs
*Array of character locations within the spritesheet
*
*@note This array is indexed by the character to be located
*/

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
