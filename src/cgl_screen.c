
#include "cgl.h"

struct CGL_Screen {
  CGL_InitScreenFunc init;
  CGL_UpdateScreenFunc update;
  CGL_RenderScreenFunc render;
  CGL_DestroyScreenFunc destroy;
  SDL_Renderer *rend;
  void *data;
};

CGL_Screen* CGL_CreateScreen(CGL_InitScreenFunc init, 
                             CGL_UpdateScreenFunc update, 
                             CGL_RenderScreenFunc render, 
                             CGL_DestroyScreenFunc destroy,
                             SDL_Renderer *rend)
{
  CGL_Screen *screen = (CGL_Screen*)malloc(sizeof(CGL_Screen));
  if(screen == NULL)
    return NULL;

  screen->init = init;
  screen->update = update;
  screen->render = render;
  screen->destroy = destroy;
  screen->rend = rend;
  return screen;
}

SDL_Renderer* CGL_ScreenGetRenderer(CGL_Screen *screen)
{
  if(screen == NULL)
    return NULL;
  return screen->rend;
}

void* CGL_ScreenGetData(CGL_Screen *screen)
{
  if(screen == NULL)
    return NULL;
  return screen->data;
}

void CGL_ScreenSetData(CGL_Screen *screen, void *data)
{
  if(screen != NULL)
    screen->data = data;
}

int CGL_ScreenInit(CGL_Screen *screen)
{
  if(screen == NULL)
    return -1;
  return screen->init(screen);
}

void CGL_ScreenUpdate(CGL_Screen *screen)
{
  if(screen != NULL)
    screen->update(screen);
}

void CGL_ScreenRender(CGL_Screen *screen)
{
  if(screen != NULL)
    screen->render(screen);
}

void CGL_DestroyScreen(CGL_Screen *screen)
{
  if(screen == NULL)
    return;

  screen->destroy(screen);
  free(screen);
}
