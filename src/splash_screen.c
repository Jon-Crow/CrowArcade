#include "splash_screen.h"

#include "resources.h"
#include "menu_screen.h"

typedef struct SplashScreenData SplashScreenData;

struct SplashScreenData {
  CGL_Animation *splash;
  int timer;
};

int SplashScreenInit(CGL_Screen *screen)
{
  CGL_SpriteSheet *sheet = ResourcesGetSpriteSheet(SPRITE_SHEET_SPLASH_SCREEN);
  CGL_Animation *splash = CGL_AnimationFromRows(sheet, 0, 2, 7, false);
  if(splash == NULL)
    return -1;
  
  SplashScreenData *data = (SplashScreenData*)malloc(sizeof(SplashScreenData));
  if(data == NULL)
  {
    CGL_DestroyAnimation(splash);
    return -1;
  }

  data->splash = splash;
  data->timer = 60*5;
  CGL_ScreenSetData(screen, data);

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
    CGL_DestroyAnimation(data->splash);
  }
  
  free(data);
}
