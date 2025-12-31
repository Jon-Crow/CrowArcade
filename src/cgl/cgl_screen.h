/**
*@file cgl_screen.h
*@brief Definition of a structure that represents a game "scene"
*
*
*@author Jonathan Crow
*@date 2025
*/

#ifndef CGL_SCREEN_H
#define CGL_SCREEN_H

#include <SDL2/SDL.h>

typedef struct CGL_Context CGL_Context;
typedef struct CGL_Screen CGL_Screen;

typedef int (*CGL_InitScreenFunc)(CGL_Screen *screen, CGL_Context *ctx);    /**< Screen init function pointer */
typedef void (*CGL_UpdateScreenFunc)(CGL_Screen *screen, CGL_Context *ctx); /**< Screen update function pointer */
typedef void (*CGL_RenderScreenFunc)(CGL_Screen *screen, CGL_Context *ctx); /**< Screen render function pointer */
typedef void (*CGL_DestroyScreenFunc)(CGL_Screen *screen);                  /**< Screen destroy function pointer */

/**
*@brief Creates a new screen
*
*@param init    Pointer to the screen's init function
*@param update  Pointer to the screen's update function
*@param render  Pointer to the screen's render function
*@param destroy Pointer to the screen's destroy function
*@param rend    Global SDL renderer handle
*
*@return A pointer to a CGL_Screen instance, or NULL if there was an error
*/
CGL_Screen* CGL_CreateScreen(CGL_InitScreenFunc init, 
                             CGL_UpdateScreenFunc update, 
                             CGL_RenderScreenFunc render, 
                             CGL_DestroyScreenFunc destroy,
                             SDL_Renderer *rend);

/**
*@param screen The screen to be referenced
*
*@return Pointer to the screen's SDL renderer
*/
SDL_Renderer* CGL_ScreenGetRenderer(CGL_Screen *screen);

/**
*@param screen The screen to be referenced
*
*@return Pointer to the screen's generic data
*/
void* CGL_ScreenGetData(CGL_Screen *screen);

/**
*@brief Sets the given screen's generic data
*
*@param screen The screen to be modified
*@param data   The data to be stored in the screen
*/
void CGL_ScreenSetData(CGL_Screen *screen, void *data);

/**
*@brief Call the given screen's init function
*
*@param screen The screen for which the init function is to be called
*/
int CGL_ScreenInit(CGL_Screen *screen, CGL_Context *ctx);

/**
*@brief Call the given screen's update function
*
*@param screen The screen for which the update function is to be called
*@param ctx    Global SDL context
*/
void CGL_ScreenUpdate(CGL_Screen *screen, CGL_Context *ctx);

/**
*@brief Call the given screen's render function
*
*@param screen The screen for which the render function is to be called
*@param ctx    Global SDL context
*/
void CGL_ScreenRender(CGL_Screen *screen, CGL_Context *ctx);

/**
*@brief Calls the given screen's destroy function and frees all memory owned by the given screen
*
*@param anim screen to destroy
*/
void CGL_DestroyScreen(CGL_Screen *screen);

#endif
