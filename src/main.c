
#include <unistd.h>

#include "macros.h"
#include "cgl.h"

typedef struct {
  CGL_Texture *splash;
  int timer;
} SplashScreenData;

int SplashScreenInit(CGL_Screen *screen)
{
  CGL_Texture *splash = CGL_LoadTexture(CGL_ScreenGetRenderer(screen), "../res/texture/stay_storm.png");
  if(splash == NULL)
    return -1;

  SplashScreenData *data = (SplashScreenData*)malloc(sizeof(SplashScreenData));
  if(data == NULL)
    return -1;

  data->splash = splash;
  data->timer = 60*5;
  CGL_ScreenSetData(screen, data);

  return 0;
}

void SplashScreenUpdate(CGL_Screen *screen)
{

}

void SplashScreenRender(CGL_Screen *screen)
{
  SplashScreenData *data = CGL_ScreenGetData(screen);

  SDL_Rect srcRect = {0, 0, CGL_TextureGetWidth(data->splash), CGL_TextureGetHeight(data->splash)};
  SDL_Rect dstRect = {0, 0, GAME_WIDTH, GAME_HEIGHT};

  SDL_RenderCopy(CGL_ScreenGetRenderer(screen), CGL_TextureGetImage(data->splash), NULL, &dstRect);
}

void SplashScreenDestroy(CGL_Screen *screen)
{
  SplashScreenData *data = CGL_ScreenGetData(screen);

  if(data == NULL)
    return;

  if(data->splash != NULL)
    CGL_DestroyTexture(data->splash);
  
  free(data);
}

int main(void)
{
  SDL_Window *win;
  SDL_Renderer *rend;
  SDL_Texture *gameTx;
  SDL_Event evt;
  CGL_Screen *screen;

  printf("Initializing SDL... ");

  if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
  {
    printf("failed: %s\n", SDL_GetError());
    return -1;
  }
  printf("done.\n");

  SDL_ShowCursor(SDL_DISABLE);

  printf("Creating window... ");
  if(SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN, &win, &rend))
  {
    printf("failed: %s\n", SDL_GetError());
    SDL_Quit();
    return -1;
  }
  printf("done.\n");

  int screenW, 
      screenH;
  SDL_GetRendererOutputSize(rend, &screenW, &screenH);
  printf("Renderer size: %d x %d\n", screenW, screenH);

  printf("Creating game texture... ");
  gameTx = SDL_CreateTexture(rend, 
                             SDL_PIXELFORMAT_RGBA8888, 
                             SDL_TEXTUREACCESS_TARGET,
                             GAME_WIDTH,
                             GAME_HEIGHT);
  if(gameTx == NULL)
  {
    printf("failed: %s\n", SDL_GetError());
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return -1;
  }
  printf("done.\n");

  printf("Creating splash screen... ");
  screen = CGL_CreateScreen(SplashScreenInit,
                            SplashScreenUpdate,
                            SplashScreenRender,
                            SplashScreenDestroy,
                            rend);
  if(screen == NULL)
  {
    printf("failed: %s\n", SDL_GetError());
    SDL_DestroyTexture(gameTx);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return -1;
  }
  printf("done.\n");

  printf("Initializing splash screen... ");
  if(CGL_ScreenInit(screen))
  {
    printf("failed: %s\n", SDL_GetError());
    CGL_DestroyScreen(screen);
    SDL_DestroyTexture(gameTx);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return -1;
  }
  printf("done.\n");

  SDL_Rect gameTxDst = {
    .w = screenH, 
    .h = screenW, 
    .x = (screenW - screenH) / 2, 
    .y = (screenH - screenW) / 2
  };
  int quit = 0;
  while(quit == 0)
  {
    while(SDL_PollEvent(&evt))
    {
      if(evt.type == SDL_KEYDOWN)
        quit = 1;
    }

    SDL_SetRenderTarget(rend, gameTx);
    SDL_RenderSetViewport(rend, NULL);

    SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
    SDL_RenderClear(rend);

    CGL_ScreenUpdate(screen);
    CGL_ScreenRender(screen);

    SDL_SetRenderTarget(rend, NULL);
    SDL_RenderSetViewport(rend, NULL);

    SDL_RenderCopyEx(rend, gameTx, NULL, &gameTxDst, 90.0f, NULL, SDL_FLIP_NONE);

    SDL_RenderPresent(rend);

    SDL_Delay(16);
  }

  printf("Exiting\n");
  CGL_DestroyScreen(screen);
  SDL_DestroyTexture(gameTx);
  SDL_DestroyRenderer(rend);
  SDL_DestroyWindow(win);
  SDL_Quit();

  return 0;
}

