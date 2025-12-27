
#include "cgl.h"

struct CGL_Texture {
  SDL_Texture *img;
  int width;
  int height;
};

struct CGL_TextureRegion {
  CGL_Texture *tx;
  SDL_Rect rect;
};

CGL_TextureRegion* CGL_InitTextureRegion()
{
  CGL_TextureRegion *reg = (CGL_TextureRegion*)malloc(sizeof(CGL_TextureRegion));
  if(reg == NULL)
    return NULL;

  reg->tx = NULL;
  reg->rect = (SDL_Rect){
    .x = 0,
    .y = 0,
    .w = 0,
    .h = 0
  };

  return reg;
}
CGL_TextureRegion* CGL_CreateTextureRegion(CGL_Texture *tx, const SDL_Rect *rect)
{
  if(tx == NULL)
    return NULL;

  CGL_TextureRegion *reg = (CGL_TextureRegion*)malloc(sizeof(CGL_TextureRegion));
  if(reg == NULL)
    return NULL;

  reg->tx = tx;
  if(rect == NULL)
  {
    reg->rect = (SDL_Rect){
      .x = 0,
      .y = 0,
      .w = tx->width,
      .h = tx->height
    };
  }
  else
  {
    reg->rect = *rect;
  }

    return reg;
}

CGL_Texture* CGL_LoadTexture(SDL_Renderer *rend, const char *path)
{
  SDL_Texture *img = IMG_LoadTexture(rend, path);
  if(img == NULL)
    return NULL;

  int width,
      height;
  if(SDL_QueryTexture(img, NULL, NULL, &width, &height))
    return NULL;

  CGL_Texture *tx = (CGL_Texture*)malloc(sizeof(CGL_Texture));
  if(tx == NULL)
    return NULL;

  tx->img = img;
  tx->width = width;
  tx->height = height;

  return tx;
}

SDL_Texture* CGL_TextureGetImage(CGL_Texture *tx)
{
  return tx->img;
}

int CGL_TextureGetWidth(CGL_Texture *tx)
{
  return tx->width;
}
int CGL_TextureGetHeight(CGL_Texture *tx)
{
  return tx->height;
}
void CGL_TextureGetSize(CGL_Texture *tx, int *width, int *height)
{
  *width = tx->width;
  *height = tx->height;
}

CGL_Texture* CGL_TextureRegionGetTexture(CGL_TextureRegion *reg)
{
  return reg->tx;
}
SDL_Texture* CGL_TextureRegionGetImage(CGL_TextureRegion *reg)
{
  CGL_Texture *tx = CGL_TextureRegionGetTexture(reg);
  return CGL_TextureGetImage(tx);
}
void CGL_TextureRegionSetTexture(CGL_TextureRegion *reg, CGL_Texture *tx)
{
  reg->tx = tx;
}

int CGL_TextureRegionGetX(CGL_TextureRegion *reg)
{
  return reg->rect.x;
}
void CGL_TextureRegionSetX(CGL_TextureRegion *reg, int x)
{
  reg->rect.x = x;
}
int CGL_TextureRegionGetY(CGL_TextureRegion *reg)
{
  return reg->rect.y;
}
void CGL_TextureRegionSetY(CGL_TextureRegion *reg, int y)
{
  reg->rect.y = y;
}
void CGL_TextureRegionGetPos(CGL_TextureRegion *reg, int *x, int *y)
{
  *x = reg->rect.x;
  *y = reg->rect.y;
}
void CGL_TextureRegionSetPos(CGL_TextureRegion *reg, int x, int y)
{
  reg->rect.x = x;
  reg->rect.y = y;
}

int CGL_TextureRegionGetWidth(CGL_TextureRegion *reg)
{
  return reg->rect.w;
}
void CGL_TextureRegionSetWidth(CGL_TextureRegion *reg, int width)
{
  reg->rect.w = width;
}
int CGL_TextureRegionGetHeight(CGL_TextureRegion *reg)
{
  return reg->rect.h;
}
void CGL_TextureRegionSetHeight(CGL_TextureRegion *reg, int height)
{
  reg->rect.h = height;
}
void CGL_TextureRegionGetSize(CGL_TextureRegion *reg, int *width, int *height)
{
  *width = reg->rect.w;
  *height = reg->rect.h;
}
void CGL_TextureRegionSetSize(CGL_TextureRegion *reg, int width, int height)
{
  reg->rect.w = width;
  reg->rect.h = height;
}

void CGL_TextureRegionGetBounds(CGL_TextureRegion *reg, int *x, int *y, int *width, int *height)
{
  *x = reg->rect.x;
  *y = reg->rect.y;
  *width = reg->rect.w;
  *height = reg->rect.h;
}
void CGL_TextureRegionSetBounds(CGL_TextureRegion *reg, int x, int y, int width, int height)
{
  reg->rect.x = x;
  reg->rect.y = y;
  reg->rect.w = width;
  reg->rect.h = height;
}
void CGL_TextureRegionGetRect(CGL_TextureRegion *reg, SDL_Rect *bounds)
{
  if(reg == NULL)
    return;
  if(bounds == NULL)
    return;
  
  bounds->x = reg->rect.x;
  bounds->y = reg->rect.y;
  bounds->w = reg->rect.w;
  bounds->h = reg->rect.h;
}
void CGL_TextureRegionSetRect(CGL_TextureRegion *reg, const SDL_Rect *bounds)
{
  reg->rect = *bounds;
}

void CGL_DestroyTexture(CGL_Texture *tx)
{
  SDL_DestroyTexture(tx->img);
  free(tx);
}

void CGL_DestroyTextureRegion(CGL_TextureRegion *reg)
{
  free(reg);
}
