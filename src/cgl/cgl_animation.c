
#include "cgl_animation.h"

struct CGL_Animation {
  CGL_TextureRegion **frames;
  size_t frameCount;
  size_t curFrame;
  int frameTime;
  int clock;
  bool loop;
  bool done;
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

  anim->frameCount = frameCount;
  anim->curFrame = 0;
  anim->frameTime = frameTime;
  anim->clock = 0;
  anim->loop = false;
  anim->done = false;

  return anim;
}

CGL_Animation* CGL_AnimationFromRows(CGL_SpriteSheet* sheet, int startRow, int endRow, int frameTime, bool loop)
{
  int cols = CGL_SpriteSheetGetColumns(sheet);
  size_t frameCount = cols * (endRow - startRow);
  printf("Creating animation. Sheet has %d columns.\nFrame count: %zu\n", cols, frameCount);
  
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

  anim->loop = loop;

  return anim;
}

CGL_TextureRegion* CGL_AnimationGetFrame(CGL_Animation *anim, size_t frame)
{
  if(anim == NULL)
    return NULL;
  if(frame >= anim->frameCount)
    return NULL;

  return anim->frames[frame];
}

CGL_TextureRegion* CGL_AnimationGetCurrentFrame(CGL_Animation *anim)
{
  if(anim == NULL)
    return NULL;
  return CGL_AnimationGetFrame(anim, anim->curFrame);
}

void CGL_AnimationUpdate(CGL_Animation *anim)
{
  anim->clock++;
  if(anim->clock >= anim->frameTime)
  {
    anim->clock = 0;
    anim->curFrame++;
    if(anim->curFrame >= anim->frameCount)
    {
      if(anim->loop)
        anim->curFrame = 0;
      else
      {
        anim->curFrame = anim->frameCount-1;
        anim->done = true;
      }
    }
  }
}

bool CGL_AnimationIsDone(CGL_Animation *anim)
{
  if(anim == NULL)
    return false;
  return anim->done;
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
        CGL_DestroyTextureRegion(anim->frames[i]);
    }
    free(anim->frames);
  }

  free(anim);
}
