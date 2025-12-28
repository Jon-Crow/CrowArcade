/**
*@file cgl_context.h
*@brief Definition of structure that contains SDL globals needed throughout the application
*
*@author Jonathan Crow
*@date 2025
*/

#ifndef CGL_CONTEXT_H
#define CGL_CONTEXT_H

#include <stdbool.h>

#include "cgl_context.h"
#include "cgl_screen.h"

#define CGL_INPUT_COUNT (4)
#define CGL_INPUT_UP (0)
#define CGL_INPUT_LEFT (1)
#define CGL_INPUT_DOWN (2)
#define CGL_INPUT_RIGHT (3)

typedef struct CGL_Context CGL_Context;

CGL_Context* CGL_CreateContext();

SDL_Window* CGL_ContextGetWindow(CGL_Context *ctx);
SDL_Renderer* CGL_ContextGetRenderer(CGL_Context *ctx);
SDL_Texture* CGL_ContextGetGameTexture(CGL_Context *ctx);
CGL_Screen* CGL_ContextGetScreen(CGL_Context *ctx);
void CGL_ContextSetScreen(CGL_Context *ctx, CGL_Screen *screen);
void CGL_ContextGetScreenSize(CGL_Context *ctx, int *screenW, int *screenH);
bool CGL_ContextGetInput(CGL_Context *ctx, size_t idx);
void CGL_ContextSetInput(CGL_Context *ctx, size_t idx, bool val);
void CGL_ContextUpdateInput(CGL_Context *ctx);
bool CGL_ContextInputJustSet(CGL_Context *ctx, size_t idx);
bool CGL_ContextInputJustReset(CGL_Context *ctx, size_t idx);

void CGL_DestroyContext(CGL_Context *ctx);

#endif
