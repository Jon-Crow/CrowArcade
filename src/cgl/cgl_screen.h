
#ifndef CGL_SCREEN_H
#define CGL_SCREEN_H

#include <SDL2/SDL.h>

typedef struct CGL_Context CGL_Context;
typedef struct CGL_Screen CGL_Screen;

typedef int (*CGL_InitScreenFunc)(CGL_Screen *screen);
typedef void (*CGL_UpdateScreenFunc)(CGL_Screen *screen, CGL_Context *ctx);
typedef void (*CGL_RenderScreenFunc)(CGL_Screen *screen, CGL_Context *ctx);
typedef void (*CGL_DestroyScreenFunc)(CGL_Screen *screen);

CGL_Screen* CGL_CreateScreen(CGL_InitScreenFunc init, 
                             CGL_UpdateScreenFunc update, 
                             CGL_RenderScreenFunc render, 
                             CGL_DestroyScreenFunc destroy,
                             SDL_Renderer *rend);

SDL_Renderer* CGL_ScreenGetRenderer(CGL_Screen *screen);
void* CGL_ScreenGetData(CGL_Screen *screen);

void CGL_ScreenSetData(CGL_Screen *screen, void *data);

int CGL_ScreenInit(CGL_Screen *screen);
void CGL_ScreenUpdate(CGL_Screen *screen, CGL_Context *ctx);
void CGL_ScreenRender(CGL_Screen *screen, CGL_Context *ctx);

void CGL_DestroyScreen(CGL_Screen *screen);

#endif
