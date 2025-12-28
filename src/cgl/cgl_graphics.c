
#include "cgl_graphics.h"

#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void CGL_DrawTexture(CGL_Context *ctx, CGL_Texture *tx, int x, int y, int w, int h)
{
  SDL_Renderer *rend = CGL_ContextGetRenderer(ctx);
  if(rend == NULL)
    return;

  SDL_Texture *img = CGL_TextureGetImage(tx);
  if(img == NULL)
    return;

  SDL_Rect dst;
  dst.x = x;
  dst.y = y;

  if(w <= 0)
    dst.w = CGL_TextureGetWidth(tx);
  else
    dst.w = w;

  if(h <= 0)
    dst.h = CGL_TextureGetHeight(tx);
  else
    dst.h = h;

  
  SDL_RenderCopy(rend, img, NULL, &dst);
}

void CGL_DrawTextureRegion(CGL_Context *ctx, CGL_TextureRegion *reg, int x, int y, int w, int h)
{
  SDL_Renderer *rend = CGL_ContextGetRenderer(ctx);
  if(rend == NULL)
    return;

  SDL_Texture *img = CGL_TextureRegionGetImage(reg);
  if(img == NULL)
    return;

  SDL_Rect src;
  CGL_TextureRegionGetRect(reg, &src);

  SDL_Rect dst;
  dst.x = x;
  dst.y = y;

  if(w <= 0)
    dst.w = CGL_TextureRegionGetWidth(reg);
  else
    dst.w = w;

  if(h <= 0)
    dst.h = CGL_TextureRegionGetHeight(reg);
  else
    dst.h = h;

  SDL_RenderCopy(rend, img, &src, &dst);
}

void CGL_DrawAnimation(CGL_Context *ctx, CGL_Animation *anim, int x, int y, int w, int h)
{
  CGL_TextureRegion *frame = CGL_AnimationGetCurrentFrame(anim);
  if(frame != NULL)
    CGL_DrawTextureRegion(ctx, frame, x, y, w, h);
}

void CGL_DrawString(CGL_Context *ctx, CGL_Font *font, const char *str, int x, int y, int charW, int charH, const SDL_Color *clr)
{
  if(font == NULL)
    return;

  CGL_SpriteSheet *sheet = CGL_FontGetSpriteSheet(font);
  if(sheet == NULL)
    return;

  SDL_Texture *img = CGL_SpriteSheetGetImage(sheet);
  if(img == NULL)
    return;

  CGL_TextureRegion *reg = CGL_InitTextureRegion();

  if(clr != NULL)
    SDL_SetTextureColorMod(img, clr->r, clr->g, clr->b);

  int len = strlen(str);
  for(int i = 0; i < len; i++)
  {
    CGL_FontGetGlyph(font, str[i], reg);
    CGL_DrawTextureRegion(ctx, reg, x+i*charW, y, charW, charH);
  }

  if(clr != NULL)
    SDL_SetTextureColorMod(img, 255, 255, 255);

  CGL_DestroyTextureRegion(reg);
}
