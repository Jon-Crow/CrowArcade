#include "splash_screen.h"

#include "menu_screen.h"

typedef struct SplashScreenData SplashScreenData;

struct SplashScreenData {
  CGL_Animation *splash;
  int timer;
};

int SplashScreenInit(CGL_Screen *screen)
{
  CGL_Texture *tx = CGL_LoadTexture(CGL_ScreenGetRenderer(screen), "../res/texture/stay_storm.png");
  if(tx == NULL)
    return -1;

  CGL_SpriteSheet *sheet = CGL_CreateSpriteSheet(tx, 224, 288);
  if(sheet == NULL)
  {
    CGL_DeepDestroyTexture(tx);
    return -1;
  }

  CGL_Animation *splash = CGL_AnimationFromRows(sheet, 0, 2, 7, false);
  if(splash == NULL)
  {
    CGL_DestroySpriteSheet(sheet);
    CGL_DeepDestroyTexture(tx);
    return -1;
  }
  
  SplashScreenData *data = (SplashScreenData*)malloc(sizeof(SplashScreenData));
  if(data == NULL)
  {
    CGL_DestroyAnimation(splash);
    CGL_DestroySpriteSheet(sheet);
    CGL_DeepDestroyTexture(tx);
    return -1;
  }

  data->splash = splash;
  data->timer = 60*5;
  CGL_ScreenSetData(screen, data);

  CGL_DestroySpriteSheet(sheet);
  return 0;
}

void SplashScreenUpdate(CGL_Screen *screen, CGL_Context *ctx)
{
  SplashScreenData *data = CGL_ScreenGetData(screen);
  CGL_AnimationUpdate(data->splash);

  if(CGL_AnimationIsDone(data->splash))
  {
    printf("switching to menu screen\n");
    CGL_Screen *menu = CGL_CreateScreen(MenuScreenInit,
                                        MenuScreenUpdate,
                                        MenuScreenRender,
                                        MenuScreenDestroy,
                                        CGL_ContextGetRenderer(ctx));
    if(menu != NULL)
      CGL_ContextSetScreen(ctx, menu);
    else
      printf("Failed to create menu screen.\n");
  }
}

void SplashScreenRender(CGL_Screen *screen, CGL_Context *ctx)
{
  SplashScreenData *data = CGL_ScreenGetData(screen);

  CGL_DrawAnimation(ctx, data->splash, 0, 0, GAME_WIDTH, GAME_HEIGHT);
}

void SplashScreenDestroy(CGL_Screen *screen)
{
  SplashScreenData *data = CGL_ScreenGetData(screen);

  if(data == NULL)
    return;

  if(data->splash != NULL)
  {
    CGL_TextureRegion *frame = CGL_AnimationGetFrame(data->splash, 0);
    if(frame != NULL)
    {
      CGL_Texture *tx = CGL_TextureRegionGetTexture(frame);
      if(tx != NULL)
      {
        SDL_Texture *img = CGL_TextureGetImage(tx);
        if(img != NULL)
          SDL_DeepDestroyTexture(img);
      }
    }
    CGL_DestroyAnimation(data->splash);
  }
  
  free(data);
}
