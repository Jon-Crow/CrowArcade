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

CGL_Animation* CGL_InitAnimation(size_t frameCount, int frameTime, bool loop);

//startRow inclusive, endRow exclusive
CGL_Animation* CGL_AnimationFromRows(CGL_SpriteSheet *sheet, int startRow, int endRow, int frameTime, bool loop);

bool CGL_AnimationIsLooping(CGL_Animation *anim);
void CGL_AnimationSetLooping(CGL_Animation *anim, bool loop);
CGL_TextureRegion* CGL_AnimationGetFrame(CGL_Animation *anim, size_t frame);
CGL_TextureRegion* CGL_AnimationGetCurrentFrame(CGL_Animation *anim);
bool CGL_AnimationIsDone(CGL_Animation *anim);

void CGL_AnimationUpdate(CGL_Animation *anim);

void CGL_DestroyAnimation(CGL_Animation *anim);

#endif