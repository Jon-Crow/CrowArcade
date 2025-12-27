
#ifndef CGL_ANIMATION_H
#define CGL_ANIMATION_H

typedef struct CGL_Animation CGL_Animation;

CGL_Animation* CGL_InitAnimation(size_t frameCount, int frameTime);

//startRow inclusive, endRow exclusive
CGL_Animation* CGL_AnimationFromRows(CGL_SpriteSheet *sheet, int startRow, int endRow, int frameTime);

CGL_TextureRegion* CGL_AnimationGetFrame(CGL_Animation *anim, size_t frame);
CGL_TextureRegion* CGL_AnimationGetCurrentFrame(CGL_Animation *anim);

void CGL_AnimationUpdate(CGL_Animation *anim);

void CGL_DestroyAnimation(CGL_Animation *anim);

#endif