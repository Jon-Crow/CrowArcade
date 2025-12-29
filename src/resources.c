
#include "resources.h"

typedef struct Resources Resources;

struct Resources {
  CGL_Texture *tx[TEXTURE_COUNT];
  CGL_SpriteSheet *sheets[SPRITE_SHEET_COUNT];
  CGL_Font *fonts[FONT_COUNT];
};

static Resources res;

bool InitResources(CGL_Context *ctx)
{
  SDL_Renderer *rend = CGL_ContextGetRenderer(ctx);
  if(rend == NULL)
    return false;

  res.tx[TEXTURE_SPLASH_SCREEN]  = CGL_LoadTexture(rend, TEXTURE_SPLASH_SCREEN_PATH);
  res.tx[TEXTURE_PAC_MAN_FONT]   = CGL_LoadTexture(rend, TEXTURE_PAC_MAN_FONT_PATH);
  res.tx[TEXTURE_PAC_MAN_PLAYER] = CGL_LoadTexture(rend, TEXTURE_PAC_MAN_PLAYER_PATH);
  res.tx[TEXTURE_PAC_MAN_GHOSTS] = CGL_LoadTexture(rend, TEXTURE_PAC_MAN_GHOSTS_PATH);
  res.tx[TEXTURE_PAC_MAN_LEVELS] = CGL_LoadTexture(rend, TEXTURE_PAC_MAN_LEVELS_PATH);
  res.tx[TEXTURE_FROGGER_FROG]   = CGL_LoadTexture(rend, TEXTURE_FROGGER_FROG_PATH);

  char strBuff[128];

  printf("Checking textures:\n");
  for(int i = 0; i < TEXTURE_COUNT; i++)
  {
    if(res.tx[i] == NULL)
    {
      printf("ERROR: Texture %d was NULL.\n", i);
      return false;
    }
    CGL_TextureToString(res.tx[i], strBuff, 128);
    printf("#%d: %s\n", i, strBuff);
  }

  res.sheets[SPRITE_SHEET_SPLASH_SCREEN]  = CGL_CreateSpriteSheet(res.tx[TEXTURE_SPLASH_SCREEN],  SPRITE_SHEET_SPLASH_SCREEN_SPRITE_WIDTH,  SPRITE_SHEET_SPLASH_SCREEN_SPRITE_HEIGHT);  /**<  */
  res.sheets[SPRITE_SHEET_PAC_MAN_FONT]   = CGL_CreateSpriteSheet(res.tx[TEXTURE_PAC_MAN_FONT],   SPRITE_SHEET_PAC_MAN_FONT_SPRITE_WIDTH,   SPRITE_SHEET_PAC_MAN_FONT_SPRITE_HEIGHT);   /**<  */
  res.sheets[SPRITE_SHEET_PAC_MAN_PLAYER] = CGL_CreateSpriteSheet(res.tx[TEXTURE_PAC_MAN_PLAYER], SPRITE_SHEET_PAC_MAN_PLAYER_SPRITE_WIDTH, SPRITE_SHEET_PAC_MAN_PLAYER_SPRITE_HEIGHT); /**<  */
  res.sheets[SPRITE_SHEET_PAC_MAN_GHOSTS] = CGL_CreateSpriteSheet(res.tx[TEXTURE_PAC_MAN_GHOSTS], SPRITE_SHEET_PAC_MAN_GHOSTS_SPRITE_WIDTH, SPRITE_SHEET_PAC_MAN_GHOSTS_SPRITE_HEIGHT); /**<  */
  res.sheets[SPRITE_SHEET_PAC_MAN_LEVELS] = CGL_CreateSpriteSheet(res.tx[TEXTURE_PAC_MAN_LEVELS], SPRITE_SHEET_PAC_MAN_LEVELS_SPRITE_WIDTH, SPRITE_SHEET_PAC_MAN_LEVELS_SPRITE_HEIGHT); /**<  */
  res.sheets[SPRITE_SHEET_FROGGER_FROG]   = CGL_CreateSpriteSheet(res.tx[TEXTURE_FROGGER_FROG],   SPRITE_SHEET_FROGGER_FROG_SPRITE_WIDTH,   SPRITE_SHEET_FROGGER_FROG_SPRITE_HEIGHT);   /**<  */

  printf("Checking sprite sheets:\n");
  for(int i = 0; i < SPRITE_SHEET_COUNT; i++)
  {
    if(res.sheets[i] == NULL)
    {
      printf("ERROR: Sprite sheet %d was NULL.\n", i);
      return false;
    }
    CGL_SpriteSheetToString(res.sheets[i], strBuff, 128);
    printf("#%d: %s\n", i, strBuff);
  }

  res.fonts[FONT_PAC_MAN] = CGL_CreateFont(res.sheets[SPRITE_SHEET_PAC_MAN_FONT], FONT_PAC_MAN_CHARS);

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

CGL_SpriteSheet* ResourcesGetSpriteSheet(size_t idx)
{
  if(idx >= SPRITE_SHEET_COUNT)
    return NULL;
  return res.sheets[idx];
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

  for(int i = 0; i < SPRITE_SHEET_COUNT; i++)
  {
    if(res.sheets[i] != NULL)
      CGL_DestroySpriteSheet(res.sheets[i]);
  }

  for(int i = 0; i < FONT_COUNT; i++)
  {
    if(res.fonts[i] != NULL)
      CGL_DestroyFont(res.fonts[i]);
  }
}

