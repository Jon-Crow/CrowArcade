
#include "cgl_context.h"

#include "cgl_macros.h"

struct CGL_Context {
  SDL_Window *win;
  SDL_Renderer *rend;
  SDL_Texture *gameTx;
  CGL_Screen *screen;
  int screenW;
  int screenH;
  bool input[CGL_INPUT_COUNT];
  bool prevInput[CGL_INPUT_COUNT];
};

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
  if(ctx->screen != NULL)
    CGL_DestroyScreen(ctx->screen);
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

