/**
*@file cgl_font.h
*@brief Definition of structure that interprets a spritesheet as a font
*
*@author Jonathan Crow
*@date 2025
*/

#ifndef CGL_FONT_H
#define CGL_FONT_H

#include "cgl_texture.h"
#include "cgl_spritesheet.h"

typedef struct CGL_Font CGL_Font;

CGL_Font* CGL_CreateFont(CGL_SpriteSheet *sheet, const char *chars);

CGL_SpriteSheet* CGL_FontGetSpriteSheet(CGL_Font *font);
void CGL_FontGetGlyph(CGL_Font *font, char ch, CGL_TextureRegion *reg);

void CGL_DestroyFont(CGL_Font *font);

#endif
