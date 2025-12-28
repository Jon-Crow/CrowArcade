
#include "resources.h"

typedef struct Resources Resources;

struct Resources {
  CGL_Texture *tx[TEXTURE_COUNT];
  CGL_Font *fonts[FONT_COUNT];
};

static Resources res;

CGL_Font* CreateFont(CGL_Texture *tx, int charW, int charH, const char *chars)
{
  CGL_SpriteSheet *sheet = CGL_CreateSpriteSheet(tx, charW, charH);
  if(sheet == NULL)
    return NULL;

  return CGL_CreateFont(sheet, chars);
}

bool InitResources(CGL_Context *ctx)
{
  SDL_Renderer *rend = CGL_ContextGetRenderer(ctx);
  if(rend == NULL)
    return false;

  res.tx[TEXTURE_SPLASH_SCREEN] = CGL_LoadTexture(rend, TEXTURE_SPLASH_SCREEN_PATH);
  res.tx[TEXTURE_PAC_MAN_FONT] = CGL_LoadTexture(rend, TEXTURE_PAC_MAN_FONT_PATH);
  res.tx[TEXTURE_PAC_MAN_GHOSTS] = CGL_LoadTexture(rend, TEXTURE_PAC_MAN_GHOSTS_PATH);
  res.tx[TEXTURE_FROGGER_FROG] = CGL_LoadTexture(rend, TEXTURE_FROGGER_FROG_PATH);

  for(int i = 0; i < TEXTURE_COUNT; i++)
  {
    if(res.tx[i] == NULL)
    {
      printf("ERROR: Texture %d was NULL.\n", i);
      return false;
    }
  }

  res.fonts[FONT_PAC_MAN] = CreateFont(res.tx[TEXTURE_PAC_MAN_FONT], FONT_PAC_MAN_CHAR_WIDTH, FONT_PAC_MAN_CHAR_HEIGHT, FONT_PAC_MAN_CHARS);

  for(int i = 0; i < FONT_COUNT; i++)
  {
    if(res.fonts[i] == NULL)
    {
      printf("ERROR: Font %d was NULL.\n", i);
      return false;
    }
  }

  return true;
}

CGL_Texture* ResourcesGetTexture(size_t idx)
{
  if(idx >= TEXTURE_COUNT)
    return NULL;
  return res.tx[idx];
}

CGL_Font* ResourcesGetFont(size_t idx)
{
  if(idx >= FONT_COUNT)
    return NULL;
  return res.fonts[idx];
}

void DestroyResources()
{
  for(int i = 0; i < TEXTURE_COUNT; i++)
  {
    if(res.tx[i] != NULL)
    {
      SDL_Texture *img = CGL_TextureGetImage(res.tx[i]);
      if(img != NULL)
        SDL_DestroyTexture(img);
      CGL_DestroyTexture(res.tx[i]);
    }
  }

  for(int i = 0; i < FONT_COUNT; i++)
  {
    if(res.fonts[i] != NULL)
    {
      CGL_SpriteSheet *sheet = CGL_FontGetSpriteSheet(res.fonts[i]);
      if(sheet != NULL)
        CGL_DestroySpriteSheet(sheet);
      CGL_DestroyFont(res.fonts[i]);
    }
  }
}

