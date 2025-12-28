/**
*@file cgl_screen.c
*@brief Definition of a structure that represents a game "scene"
*
*
*@author Jonathan Crow
*@date 2025
*/

#include "cgl_screen.h"

/**
*@brief Structure to function pointers specific to each screen
*/
struct CGL_Screen {
  CGL_InitScreenFunc init;       /**< Init function pointer */
  CGL_UpdateScreenFunc update;   /**< Update function pointer */
  CGL_RenderScreenFunc render;   /**< Render function pointer */
  CGL_DestroyScreenFunc destroy; /**< Destroy function pointer */
  SDL_Renderer *rend;
  void *data;
};

/**
*@var CGL_Screen::rend
*Pointer to the global SDL renderer
*
*@todo This should probably be removed. No need to have it since I added it to CGL_Context
*/

/**
*@var CGL_Screen::data
*Generic pointer to be maintained by each screen implementation
*
*@warning The screen implementation is responsible for allocating and freeing this memory
*/

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

void CGL_ScreenUpdate(CGL_Screen *screen, CGL_Context *ctx)
{
  if(screen != NULL)
    screen->update(screen, ctx);
}

void CGL_ScreenRender(CGL_Screen *screen, CGL_Context *ctx)
{
  if(screen != NULL)
    screen->render(screen, ctx);
}

void CGL_DestroyScreen(CGL_Screen *screen)
{
  if(screen == NULL)
    return;

  screen->destroy(screen);
  free(screen);
}
