
#include <stdbool.h>
#include <unistd.h>

#include "cgl/cgl.h"

#include "resources.h"
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

  printf("Initializing resources... ");
  if(!InitResources(ctx))
  {
    printf("failed: %s\n", SDL_GetError());
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

  CGL_ContextSetScreen(ctx, screen);

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

  const uint32_t fps = 60;
  const uint32_t frameDelay = (int)(1000.0f/fps + 0.5f);

  SDL_Rect gameTxDst = {
    .w = screenH, 
    .h = screenW, 
    .x = (screenW - screenH) / 2, 
    .y = (screenH - screenW) / 2
  };
  bool run = true;

  while(run)
  {
    uint32_t frameStart = SDL_GetTicks();

    while(SDL_PollEvent(&evt))
    {
      if(evt.type == SDL_KEYDOWN)
      {
        switch(evt.key.keysym.sym)
        {
          case SDLK_ESCAPE:
            run = false;
            break;
          
          case SDLK_w:
          case SDLK_UP:
            CGL_ContextSetInput(ctx, CGL_INPUT_UP, true);
            break;

          case SDLK_a:
          case SDLK_LEFT:
            CGL_ContextSetInput(ctx, CGL_INPUT_LEFT, true);
            break;

          case SDLK_s:
          case SDLK_DOWN:
            CGL_ContextSetInput(ctx, CGL_INPUT_DOWN, true);
            break;

          case SDLK_d:
          case SDLK_RIGHT:
            CGL_ContextSetInput(ctx, CGL_INPUT_RIGHT, true);
            break;
        }
      }
      else if(evt.type == SDL_KEYUP)
      {
        switch(evt.key.keysym.sym)
        {
          case SDLK_w:
          case SDLK_UP:
            CGL_ContextSetInput(ctx, CGL_INPUT_UP, false);
            break;

          case SDLK_a:
          case SDLK_LEFT:
            CGL_ContextSetInput(ctx, CGL_INPUT_LEFT, false);
            break;

          case SDLK_s:
          case SDLK_DOWN:
            CGL_ContextSetInput(ctx, CGL_INPUT_DOWN, false);
            break;

          case SDLK_d:
          case SDLK_RIGHT:
            CGL_ContextSetInput(ctx, CGL_INPUT_RIGHT, false);
            break;
        }
      }
    }

    screen = CGL_ContextGetScreen(ctx);

    SDL_SetRenderTarget(rend, gameTx);
    SDL_RenderSetViewport(rend, NULL);

    SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
    SDL_RenderClear(rend);

    CGL_ScreenUpdate(screen, ctx);
    screen = CGL_ContextGetScreen(ctx);
    CGL_ScreenRender(screen, ctx);

    SDL_SetRenderTarget(rend, NULL);
    SDL_RenderSetViewport(rend, NULL);

    SDL_RenderCopyEx(rend, gameTx, NULL, &gameTxDst, 90.0f, NULL, SDL_FLIP_NONE);

    SDL_RenderPresent(rend);

    CGL_ContextUpdateInput(ctx);

    uint32_t frameTime = SDL_GetTicks() - frameStart;
    if(frameTime < frameDelay)
      SDL_Delay(frameDelay - frameTime);
  }

  printf("Exiting\n");
  DestroyResources();
  CGL_DestroyContext(ctx);
  SDL_Quit();

  return 0;
}

