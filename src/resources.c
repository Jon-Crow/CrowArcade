
#include "resources.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct Resources Resources;

struct Resources {
  CGL_Texture *tx[TEXTURE_COUNT];
  CGL_SpriteSheet *sheets[SPRITE_SHEET_COUNT];
  CGL_Font *fonts[FONT_COUNT];
  CGL_Sound *sounds[SOUND_COUNT];
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

  CGL_LogInfo("Checking textures:");
  for(int i = 0; i < TEXTURE_COUNT; i++)
  {
    if(res.tx[i] == NULL)
    {
      CGL_LogError("Texture %d was NULL.", i);
      return false;
    }
    CGL_TextureToString(res.tx[i], strBuff, 128);
    CGL_LogInfo("#%d: %s", i, strBuff);
  }

  res.sheets[SPRITE_SHEET_SPLASH_SCREEN]  = CGL_CreateSpriteSheet(res.tx[TEXTURE_SPLASH_SCREEN],  SPRITE_SHEET_SPLASH_SCREEN_SPRITE_WIDTH,  SPRITE_SHEET_SPLASH_SCREEN_SPRITE_HEIGHT);  /**<  */
  res.sheets[SPRITE_SHEET_PAC_MAN_FONT]   = CGL_CreateSpriteSheet(res.tx[TEXTURE_PAC_MAN_FONT],   SPRITE_SHEET_PAC_MAN_FONT_SPRITE_WIDTH,   SPRITE_SHEET_PAC_MAN_FONT_SPRITE_HEIGHT);   /**<  */
  res.sheets[SPRITE_SHEET_PAC_MAN_PLAYER] = CGL_CreateSpriteSheet(res.tx[TEXTURE_PAC_MAN_PLAYER], SPRITE_SHEET_PAC_MAN_PLAYER_SPRITE_WIDTH, SPRITE_SHEET_PAC_MAN_PLAYER_SPRITE_HEIGHT); /**<  */
  res.sheets[SPRITE_SHEET_PAC_MAN_GHOSTS] = CGL_CreateSpriteSheet(res.tx[TEXTURE_PAC_MAN_GHOSTS], SPRITE_SHEET_PAC_MAN_GHOSTS_SPRITE_WIDTH, SPRITE_SHEET_PAC_MAN_GHOSTS_SPRITE_HEIGHT); /**<  */
  res.sheets[SPRITE_SHEET_PAC_MAN_LEVELS] = CGL_CreateSpriteSheet(res.tx[TEXTURE_PAC_MAN_LEVELS], SPRITE_SHEET_PAC_MAN_LEVELS_SPRITE_WIDTH, SPRITE_SHEET_PAC_MAN_LEVELS_SPRITE_HEIGHT); /**<  */
  res.sheets[SPRITE_SHEET_FROGGER_FROG]   = CGL_CreateSpriteSheet(res.tx[TEXTURE_FROGGER_FROG],   SPRITE_SHEET_FROGGER_FROG_SPRITE_WIDTH,   SPRITE_SHEET_FROGGER_FROG_SPRITE_HEIGHT);   /**<  */

  CGL_LogInfo("Checking sprite sheets:");
  for(int i = 0; i < SPRITE_SHEET_COUNT; i++)
  {
    if(res.sheets[i] == NULL)
    {
      CGL_LogError("Sprite sheet %d was NULL.", i);
      return false;
    }
    CGL_SpriteSheetToString(res.sheets[i], strBuff, 128);
    CGL_LogInfo("#%d: %s", i, strBuff);
  }

  res.fonts[FONT_PAC_MAN] = CGL_CreateFont(res.sheets[SPRITE_SHEET_PAC_MAN_FONT], FONT_PAC_MAN_CHARS);

  for(int i = 0; i < FONT_COUNT; i++)
  {
    if(res.fonts[i] == NULL)
    {
      CGL_LogError("Font %d was NULL.", i);
      return false;
    }
  }

  res.sounds[SOUND_PAC_MAN_START] = CGL_LoadSound(SOUND_PAC_MAN_START_PATH, CGL_ContextGetAudioDeviceID(ctx));

  CGL_LogInfo("Checking sounds:");
  for(int i = 0; i < SOUND_COUNT; i++)
  {
    if(res.sounds[i] == NULL)
    {
      CGL_LogError("Sound %d was NULL.", i);
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

CGL_Sound* ResourcesGetSound(size_t idx)
{
  if(idx >= SOUND_COUNT)
    return NULL;
  return res.sounds[idx];
}

cJSON* ResourcesParseJsonFile(const char *path)
{
  FILE *f = fopen(path, "r");
  if(f == NULL)
    return NULL;

  fseek(f, 0, SEEK_END);
  long len = ftell(f);
  fseek(f, 0, SEEK_SET);

  char *jsonStr = (char*)malloc(len + 1);
  if(jsonStr == NULL)
  {
    fclose(f);
    return NULL;
  }

  fread(jsonStr, 1, len, f);
  jsonStr[len] = '\0';
  fclose(f);

  cJSON *json = cJSON_Parse(jsonStr);
  free(jsonStr);

  return json;
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

  for(int i = 0; i < SOUND_COUNT; i++)
  {
    if(res.sounds[i] != NULL)
      CGL_DestroySound(res.sounds[i]);
  }
}

