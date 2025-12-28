/**
*@file cgl_animation.h
*@brief Implements a structure for controlling animations
*
*@author Jonathan Crow
*@date 2025
*/

#ifndef CGL_ANIMATION_H
#define CGL_ANIMATION_H

#include <stdbool.h>

#include "cgl_spritesheet.h"

typedef struct CGL_Animation CGL_Animation;

/**
*@brief Creates a new animation
*
*@param frameCount Number of frames the animation will have
*@param frameTime  Amount of time (in game frames) to remain on each frame
*@param loop       Wether or not the anumation repeats (if false, the animation will remain on the last frame)
*
*@return A pointer to a CGL_Animation instance, or NULL if there was an error
*/
CGL_Animation* CGL_InitAnimation(size_t frameCount, int frameTime, bool loop);

/**
*@brief Creates an animation from the given rows of a spritesheet
*
*@param sheet     Pointer to the spritesheet to be used
*@param startRow  First column to use in the animation (inclusive)
*@param endRow    Last column to use in the animation (exclusive)
*@param frameTime Amount of time (in game frames) to remain on each frame
*@param loop      Wether or not the anumation repeats (if false, the animation will remain on the last frame)
*
*@return A pointer to a CGL_Animation instance, or NULL if there was an error
*/
CGL_Animation* CGL_AnimationFromRows(CGL_SpriteSheet *sheet, int startRow, int endRow, int frameTime, bool loop);

/**
*@return True, if the animation loops. Otherwise, false.
*/
bool CGL_AnimationIsLooping(CGL_Animation *anim);

/**
*@param anim Animation to modify
*@param loop Wether or not the anumation repeats (if false, the animation will remain on the last frame)
*/
void CGL_AnimationSetLooping(CGL_Animation *anim, bool loop);

/**
*@return Pointer to the frame at the given index of the animation
*
*@warning This pointer is owned by the animation instance. Do not destroy.
*
*@param anim  Animation to reference
*@param frame index of the frame to retrieve
*/
CGL_TextureRegion* CGL_AnimationGetFrame(CGL_Animation *anim, size_t frame);

/**
*@return Pointer to the frame at the current index of the animation
*
*@warning This pointer is owned by the animation instance. Do not destroy.
*
*@param anim Animation to reference
*/
CGL_TextureRegion* CGL_AnimationGetCurrentFrame(CGL_Animation *anim);

/**
*@return True, if the animation is done. Otherwise, false.
*/
bool CGL_AnimationIsDone(CGL_Animation *anim);

/**
*@brief Advances the animation's state. Will update frame index when necessary.
*
*@param anim Animation to update
*/
void CGL_AnimationUpdate(CGL_Animation *anim);

/**
*@brief Frees all memory owned by the given animation
*
*@param anim Animation to destroy
*/
void CGL_DestroyAnimation(CGL_Animation *anim);

#endif