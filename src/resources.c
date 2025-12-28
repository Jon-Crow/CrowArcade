
#include "resources.h"

struct Resources {
  CGL_Texture *tx[TEXTURE_COUNT];
};

static Resources res;

bool InitResources(CGL_Context *ctx)
{
  SDL_Renderer *rend = CGL_ContextGetRenderer(ctx);
  if(rend == NULL)
    return false;

  res.tx[TEXTURE_SPLASH_SCREEN] = CGL_LoadTexture(rend, TEXTURE_SPLASH_SCREEN_PATH);
  res.tx[TEXTURE_PAC_MAN_FONT] = CGL_LoadTexture(rend, TEXTURE_PAC_MAN_FONT_PATH);
  res.tx[TEXTURE_PAC_MAN_GHOSTS] = CGL_LoadTexture(rend, TEXTURE_PAC_MAN_GHOSTS_PATH);
  res.tx[TEXTURE_FROGGER_GROG] = CGL_LoadTexture(rend, TEXTURE_FROGGER_GROG_PATH);

  for(int i = 0; i < TEXTURE_COUNT; i++)
  {
    if(res.tx[i] == NULL)
      return false;
  }

  return true;
}

CGL_Texture* ResourcesGetTexture(size_t idx)
{
  if(idx >= TEXTURE_COUNT)
    return NULL;
  return res.tx[idx];
}

void DestroyResources()
{
  for(int i = 0; i < TEXTURE_COUNT; i++)
    CGL_DeepDestroyTexture(res.tx[i]);
}

