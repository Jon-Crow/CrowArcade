
#include "cgl_graphics.h"

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
