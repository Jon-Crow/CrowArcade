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

/**
*@brief Creates a new font
*
*@param sheet The sheet from which the font is to be made
*@param chars A string that indicates each character (sequentially), starting at (0,0), that is illustrated by the provided spritesheet
*
*@return A pointer to a CGL_Font instance, or NULL if there was an error
*/
CGL_Font* CGL_CreateFont(CGL_SpriteSheet *sheet, const char *chars);

/**
*@param font The font to be referenced
*
*@return The font's spritesheet
*/
CGL_SpriteSheet* CGL_FontGetSpriteSheet(CGL_Font *font);

/**
*@brief Modifies the provided texture region to represent the texture and bounds of the provided character
*
*@param font The font to be referenced
*@param ch   The character to retrieve from the font
*@param reg  The texture region to be modified
*/
void CGL_FontGetGlyph(CGL_Font *font, char ch, CGL_TextureRegion *reg);

/**
*@brief Frees all memory owned by the given font
*
*@param ctx Font to destroy
*/
void CGL_DestroyFont(CGL_Font *font);

#endif
