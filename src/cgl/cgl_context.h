
#ifndef CGL_CONTEXT_H
#define CGL_CONTEXT_H

#include "cgl_context.h"
#include "cgl_screen.h"

typedef struct CGL_Context CGL_Context;

CGL_Context* CGL_CreateContext();

SDL_Window* CGL_ContextGetWindow(CGL_Context *ctx);
SDL_Renderer* CGL_ContextGetRenderer(CGL_Context *ctx);
SDL_Texture* CGL_ContextGetGameTexture(CGL_Context *ctx);
CGL_Screen* CGL_ContextGetScreen(CGL_Context *ctx);
void CGL_ContextSetScreen(CGL_Context *ctx, CGL_Screen *screen);
void CGL_ContextGetScreenSize(CGL_Context *ctx, int *screenW, int *screenH);

void CGL_DestroyContext(CGL_Context *ctx);

#endif
