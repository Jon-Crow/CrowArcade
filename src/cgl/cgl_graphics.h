/**
*@file cgl_graphics.h
*@brief A collection of rendering helper functions
*
*@author Jonathan Crow
*@date 2025
*/

#ifndef CGL_GRAPHICS_H
#define CGL_GRAPHICS_H

#include "cgl_context.h"
#include "cgl_texture.h"
#include "cgl_animation.h"
#include "cgl_font.h"

void CGL_GraphicsDrawFilledRect(CGL_Context *ctx, int x, int y, int w, int h, const SDL_Color *clr);
void CGL_GraphicsDrawFilledCircle(CGL_Context *ctx, int x, int y, int r, const SDL_Color *clr);

/**
*@brief Helper function to handle SDL calls necessary for drawing a texture
*
*@param ctx Context with relevant SDL handles
*@param tx  Texture to be drawn
*@param x   X position of the desired screen location
*@param y   Y position of the desired screen location
*@param w   Width of the desired screen location
*@param h   Height of the desired screen location
*/
void CGL_DrawTexture(CGL_Context *ctx, CGL_Texture *tx, int x, int y, int w, int h);

/**
*@brief Helper function to handle SDL calls necessary for drawing a texture region
*
*@param ctx Context with relevant SDL handles
*@param reg Texture region to be drawn
*@param x   X position of the desired screen location
*@param y   Y position of the desired screen location
*@param w   Width of the desired screen location
*@param h   Height of the desired screen location
*/
void CGL_DrawTextureRegion(CGL_Context *ctx, CGL_TextureRegion *reg, int x, int y, int w, int h);

/**
*@brief Helper function to handle SDL calls necessary for drawing the current frame of the given animation
*
*@param ctx  Context with relevant SDL handles
*@param anim Animation to be drawn
*@param x    X position of the desired screen location
*@param y    Y position of the desired screen location
*@param w    Width of the desired screen location
*@param h    Height of the desired screen location
*/
void CGL_DrawAnimation(CGL_Context *ctx, CGL_Animation *anim, int x, int y, int w, int h);

/**
*@brief Helper function to handle SDL calls and position for drawing the glyphs that represent a given string
*
*@param ctx   Context with relevant SDL handles
*@param str   String to be drawn
*@param x     X position of the desired screen location
*@param y     Y position of the desired screen location
*@param charW Width of each character glyph
*@param charH Height of each character glyph
*@param clr   The tint to be applied to each glyph. NULL if no tint is to be added
*/
void CGL_DrawString(CGL_Context *ctx, CGL_Font *font, const char *str, int x, int y, int charW, int charH, const SDL_Color *clr);

#endif
