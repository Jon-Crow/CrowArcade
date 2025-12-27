
#include <unistd.h>

#include "cgl/cgl.h"

#include "splash_screen.h"

int main(void)
{
  CGL_Context *ctx;
  SDL_Window *win;
  SDL_Renderer *rend;
  SDL_Texture *gameTx;
  CGL_Screen *screen;
  SDL_Event evt;
  int screenW;
  int screenH;

  printf("Initializing SDL... ");

  if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
  {
    printf("failed: %s\n", SDL_GetError());
    return -1;
  }
  printf("done.\n");

  SDL_ShowCursor(SDL_DISABLE);

  ctx = CGL_CreateContext();
  if(ctx == NULL)
  {
    SDL_Quit();
    return -1;
  }

  win = CGL_ContextGetWindow(ctx);
  rend = CGL_ContextGetRenderer(ctx);
  gameTx = CGL_ContextGetGameTexture(ctx);
  CGL_ContextGetScreenSize(ctx, &screenW, &screenH);

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
  CGL_DestroyContext(ctx);
  SDL_Quit();

  return 0;
}

