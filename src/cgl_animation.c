
#include "cgl.h"

struct CGL_Animation {
  CGL_TextureRegion **frames;
  size_t frameCount;
  int curFrame;
  int frameTime;
};

/*
CGL_Animation* CGL_InitAnimation(size_t frameCount)
{
  CGL_Animation* anim = (CGL_Animation*)malloc(sizeof(CGL_Animation));
  if(anim == NULL)
    return NULL;

  anim->frames = (CGL_TextureRegion**)malloc(frameCount * sizeof(CGL_TextureRegion*));
  if(anim->frames == NULL)
  {
    free(anim);
    return NULL;
  }

  for(size_t i = 0; i < frameCount; i++)
  {
    anim->frames[i] = (CGL_TextureRegion*)malloc(sizeof(CGL_TextureRegion));
    if(anim->frames[i] == NULL)
    {
      for(size_t x = i-1; x >= 0; x--)
        free(anim->frames[i]);
      free(anim->frames);
      free(anim);
      return;
    }
  }

  return anim;
}
*/

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
