
#ifndef CGL_GRAPHICS_H
#define CGL_GRAPHICS_H

#include "cgl_context.h"
#include "cgl_texture.h"
#include "cgl_animation.h"

void CGL_DrawTexture(CGL_Context *ctx, CGL_Texture *tx, int x, int y, int w, int h);
void CGL_DrawTextureRegion(CGL_Context *ctx, CGL_TextureRegion *reg, int x, int y, int w, int h);
void CGL_DrawAnimation(CGL_Context *ctx, CGL_Animation *anim, int x, int y, int w, int h);

#endif
