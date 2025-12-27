
#include "cgl.h"

struct CGL_Animation {
  CGL_TextureRegion **frames;
  size_t frameCount;
  int curFrame;
  int frameTime;
};

CGL_Animation* CGL_InitAnimation(size_t frameCount, int frameTime)
{
  CGL_Animation* anim = (CGL_Animation*)malloc(sizeof(CGL_Animation));
  if(anim == NULL)
    return NULL;

  anim->frames = (CGL_TextureRegion**)malloc(frameCount * sizeof(CGL_TextureRegion*));
  if(anim->frames == NULL)
    return NULL;

  for(size_t i = 0; i < frameCount; i++)
  {
    anim->frames[i] = CGL_InitTextureRegion();
    if(anim->frames[i] == NULL)
      return NULL;
  }

  anim->curFrame = 0;
  anim->frameTime = frameTime;

  return anim;
}

CGL_Animation* CGL_AnimationFromRows(CGL_SpriteSheet* sheet, int startRow, int endRow, int frameTime)
{
  int cols = CGL_SpriteSheetGetColumns(sheet);
  size_t frameCount = cols * (endRow - startRow);
  
  CGL_Animation *anim = CGL_InitAnimation(frameCount, frameTime);
  if(anim == NULL)
    return NULL;

  int frameIdx = 0;
  for(int y = startRow; y < endRow; y++)
  {
    for(int x = 0; x < cols; x++)
    {
      anim->frames[frameIdx] = CGL_SpriteSheetCreateSpriteAt(sheet, x, y);
      frameIdx++;
    }
  }

  return anim;
}

void CGL_DestroyAnimation(CGL_Animation *anim)
{
  if(anim == NULL)
    return;

  if(anim->frames != NULL)
  {
    for(size_t i = 0; i < anim->frameCount; i++)
    {
      if(anim->frames[i] != NULL)
        free(anim->frames[i]);
    }
    free(anim->frames);
  }

  free(anim);
}
