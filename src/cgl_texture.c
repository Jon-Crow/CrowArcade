
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

void CGL_DestroyTexture(CGL_Texture *tx)
{
  SDL_DestroyTexture(tx->img);
  free(tx);
}

void CGL_DestroyTextureRegion(CGL_TextureRegion *reg)
{
  free(reg);
}
