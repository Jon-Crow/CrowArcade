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

#define CGL_INPUT_COUNT (4) /**< Total number of inputs */
#define CGL_INPUT_UP    (0) /**< Joystick up */
#define CGL_INPUT_LEFT  (1) /**< Joystick left */
#define CGL_INPUT_DOWN  (2) /**< Joystick down */
#define CGL_INPUT_RIGHT (3) /**< Joystick right */

typedef struct CGL_Context CGL_Context;

/**
*@brief Creates a new context
*
*@return A pointer to a CGL_Context instance, or NULL if there was an error
*/
CGL_Context* CGL_CreateContext();

/**
*@param ctx Context to be referenced
*
*@return Pointer the the context's SDL window handle
*/
SDL_Window* CGL_ContextGetWindow(CGL_Context *ctx);

/**
*@param ctx Context to be referenced
*
*@return Pointer the the context's SDL renderer handle
*/
SDL_Renderer* CGL_ContextGetRenderer(CGL_Context *ctx);

/**
*@param ctx Context to be referenced
*
*@see CGL_Context struct's gameTx field
*
*@return Pointer the the context's game texture handle
*/
SDL_Texture* CGL_ContextGetGameTexture(CGL_Context *ctx);

/**
*@param ctx Context to be referenced
*
*@return Pointer the the context's screen handle
*/
CGL_Screen* CGL_ContextGetScreen(CGL_Context *ctx);

/**
*@param ctx    Context to be modified
*@param screen The updated value of the context's screen
*/
void CGL_ContextSetScreen(CGL_Context *ctx, CGL_Screen *screen);

/**
*@param ctx     Context to be referenced
*@param screenW The returned width of the screen
*@param screenH The returned height of the screen
*/
void CGL_ContextGetScreenSize(CGL_Context *ctx, int *screenW, int *screenH);

/**
*@return True, if the input at the given index is currently being entered. False, otherwise.
*
*@see CGL_INPUT_UP, CGL_INPUT_LEFT, CGL_INPUT_DOWN, CGL_INPUT_RIGHT
*
*@param ctx Context to be referenced
*@param idx Index of the input to be queried
*/
bool CGL_ContextGetInput(CGL_Context *ctx, size_t idx);

/**
*@see CGL_INPUT_UP, CGL_INPUT_LEFT, CGL_INPUT_DOWN, CGL_INPUT_RIGHT
*
*@param ctx Context to be modified
*@param idx Index of the input to be modified
*@param val New value of the input (true if applied)
*/
void CGL_ContextSetInput(CGL_Context *ctx, size_t idx, bool val);

/**
*@brief Copes the values of the current input values into the internal "previous" input values. Should be called at the end of every frame.
*
*@param ctx Context for which the inputs are to be swapped
*/
void CGL_ContextUpdateInput(CGL_Context *ctx);

/**
*@return True, if the input at the given index was entered this frame. False, otherwise.
*
*@see CGL_INPUT_UP, CGL_INPUT_LEFT, CGL_INPUT_DOWN, CGL_INPUT_RIGHT
*
*@param ctx Context to be referenced
*@param idx Index of the input to be queried
*/
bool CGL_ContextInputJustSet(CGL_Context *ctx, size_t idx);

/**
*@return True, if the input at the given index was released this frame. False, otherwise.
*
*@see CGL_INPUT_UP, CGL_INPUT_LEFT, CGL_INPUT_DOWN, CGL_INPUT_RIGHT
*
*@param ctx Context to be referenced
*@param idx Index of the input to be queried
*/
bool CGL_ContextInputJustReset(CGL_Context *ctx, size_t idx);

/**
*@brief Frees all memory owned by the given context
*
*@param ctx Context to destroy
*/
void CGL_DestroyContext(CGL_Context *ctx);

#endif
