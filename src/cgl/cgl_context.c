/**
*@file cgl_context.c
*@brief Definition of structure that contains SDL globals needed throughout the application
*
*@author Jonathan Crow
*@date 2025
*/

#include "cgl_context.h"

#include "cgl_macros.h"

/**
*@brief Holds SDL globals necessary throughout the application
*/
struct CGL_Context {
  SDL_Window *win;                 /**< Handle to the SDL window */
  SDL_Renderer *rend;              /**< Handle to the SDL renderer */
  SDL_Texture *gameTx;
  CGL_Screen *screen;              /**< Handle to the screen that is currently being updated/rendered */
  int screenW;                     /**< Width of the SDL window */
  int screenH;                     /**< Height of the SDL window */
  bool input[CGL_INPUT_COUNT];     /**< Current state of each input */
  bool prevInput[CGL_INPUT_COUNT]; /**< Previous frame's state of each input */
};

/**
*@var CGL_Context::gameTx
*This texture is what is actually drawn to. It is done this way so that, at the end of each frame,
*any effects can be applied. For example, in the context of the arcade emulator, the frame is rotated
*90 degrees and stretched to match the size of the monitor.
*
*@todo Make the size of this texture adjustable (duh)
*/

CGL_Context* CGL_CreateContext()
{
  CGL_Context *ctx = (CGL_Context*)malloc(sizeof(CGL_Context));
  if(ctx == NULL)
    return NULL;

  ctx->win = NULL;
  ctx->rend = NULL;
  ctx->gameTx = NULL;
  ctx->screen = NULL;

  for(int i = 0; i < CGL_INPUT_COUNT; i++)
  {
    ctx->input[i] = false;
    ctx->prevInput[i] = false;
  }

  printf("Creating window... ");
  if(SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN, &(ctx->win), &(ctx->rend)))
  {
    printf("failed: %s\n", SDL_GetError());
    return NULL;
  }
  printf("done.\n");

  SDL_GetRendererOutputSize(ctx->rend, &(ctx->screenW), &(ctx->screenH));

  printf("Creating game texture... ");
  ctx->gameTx = SDL_CreateTexture(ctx->rend, 
                                  SDL_PIXELFORMAT_RGBA8888, 
                                  SDL_TEXTUREACCESS_TARGET,
                                  GAME_WIDTH,
                                  GAME_HEIGHT);
  if(ctx->gameTx == NULL)
  {
    printf("failed: %s\n", SDL_GetError());
    return NULL;
  }
  printf("done.\n");

  ctx->screen = NULL;

  return ctx;
}

SDL_Window* CGL_ContextGetWindow(CGL_Context *ctx)
{
  return ctx->win;
}

SDL_Renderer* CGL_ContextGetRenderer(CGL_Context *ctx)
{
  return ctx->rend;
}

SDL_Texture* CGL_ContextGetGameTexture(CGL_Context *ctx)
{
  return ctx->gameTx;
}

CGL_Screen* CGL_ContextGetScreen(CGL_Context *ctx)
{
  return ctx->screen;
}

void CGL_ContextSetScreen(CGL_Context *ctx, CGL_Screen *screen)
{
  ctx->screen = screen;
}

void CGL_ContextGetScreenSize(CGL_Context *ctx, int *screenW, int *screenH)
{
  *screenW = ctx->screenW;
  *screenH = ctx->screenH;
}

bool CGL_ContextGetInput(CGL_Context *ctx, size_t idx)
{
  if(idx >= CGL_INPUT_COUNT)
    return false;
  return ctx->input[idx];
}

void CGL_ContextSetInput(CGL_Context *ctx, size_t idx, bool val)
{
  if(idx < CGL_INPUT_COUNT)
    ctx->input[idx] = val;
}

void CGL_ContextUpdateInput(CGL_Context *ctx)
{
  for(int i = 0; i < CGL_INPUT_COUNT; i++)
    ctx->prevInput[i] = ctx->input[i];
}

bool CGL_ContextInputJustSet(CGL_Context *ctx, size_t idx)
{
  if(idx >= CGL_INPUT_COUNT)
    return false;
  return ctx->input[idx] && !ctx->prevInput[idx];
}

bool CGL_ContextInputJustReset(CGL_Context *ctx, size_t idx)
{
  if(idx >= CGL_INPUT_COUNT)
    return false;
  return !ctx->input[idx] && ctx->prevInput[idx];
}

void CGL_DestroyContext(CGL_Context *ctx)
{
  if(ctx == NULL)
    return;

  if(ctx->screen != NULL)
    CGL_DestroyScreen(ctx->screen);
  if(ctx->gameTx != NULL)
    SDL_DestroyTexture(ctx->gameTx);
  if(ctx->rend != NULL)
    SDL_DestroyRenderer(ctx->rend);
  if(ctx->win != NULL)
    SDL_DestroyWindow(ctx->win);
}

