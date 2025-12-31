/**
*@file main.c
*
*@author Jonathan Crow
*@date 2025
*/

/**
*@mainpage Crow Arcade - A low level C application to emulate a Ms. Pac-Man arcade cabinet
*
*This application implements many recreations of old arcade games to be run on a 1980s Ms. Pac-Man
*arcade cabinet. It is designed to be run on a Raspberry Pi connected to a generic computer monitor 
*that is mounted in place of the cabiner's CRT monitor. It includes a graphics library called CGL
*(Crow Graphics Library) that serves as an abstraction layer for SDL.
*
*## Style
*Each data type within the CGL library uses a locally defined struct with a forward declaration in
*the corresponding header. This sacrifices some conveniece for strong encapsulation. It also forces
*the user to allocate the types on the stack, and prevents them from allocating on the stack. While,
*this is a minor drawback, the data encapsulation and memory management offered make for an easier
*to use codebase.
*
*Each type includes global function with the format: CGL_[type name][function descripton]  
*For example: CGL_TextureGetWidth  
*The exceptions to this rule are init, create, and destroy functions.  
*Those follow the following format: CGL_[Init/Create/Destroy][type name]  
*I can't really say for certain why I made those exceptions, but... I did.  
*
*## Instructions
*1. Clone the repository to your Raspberry Pi
```bash
git clone https://github.com/Jon-Crow/CrowArcade.git
```
*2. Within the root directory, use make to compile the project  
*for debug build:
```bash
make compile
```
*for release build:
```bash
make compile
```
*3. Use make to run the application
```bash
make run
```
*/

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

  CGL_LogInfo("Initializing SDL... ");

  if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
  {
    CGL_LogError("failed: %s", SDL_GetError());
    return -1;
  }

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
  
  SDL_RenderSetIntegerScale(rend, SDL_TRUE);

  CGL_LogInfo("Initializing resources... ");
  if(!InitResources(ctx))
  {
    CGL_LogError("failed: %s", SDL_GetError());
    return -1;
  }

  CGL_LogInfo("Creating splash screen... ");
  screen = CGL_CreateScreen(SplashScreenInit,
                            SplashScreenUpdate,
                            SplashScreenRender,
                            SplashScreenDestroy,
                            rend);
  if(screen == NULL)
  {
    CGL_LogError("failed: %s", SDL_GetError());
    SDL_DestroyTexture(gameTx);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return -1;
  }

  CGL_ScreenInit(screen);
  CGL_ContextSetScreen(ctx, screen);

  CGL_LogInfo("Initializing splash screen... ");
  if(CGL_ScreenInit(screen))
  {
    CGL_LogError("failed: %s", SDL_GetError());
    CGL_DestroyScreen(screen);
    SDL_DestroyTexture(gameTx);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return -1;
  }

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

          case SDLK_RETURN:
            CGL_ContextSetInput(ctx, CGL_INPUT_P1_START, true);
            break;

          case SDLK_SPACE:
            CGL_ContextSetInput(ctx, CGL_INPUT_P2_START, true);
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
          
          case SDLK_RETURN:
            CGL_ContextSetInput(ctx, CGL_INPUT_P1_START, false);
            break;

          case SDLK_SPACE:
            CGL_ContextSetInput(ctx, CGL_INPUT_P2_START, false);
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

  CGL_LogInfo("Exiting");
  DestroyResources();
  CGL_DestroyContext(ctx);
  SDL_Quit();

  return 0;
}

