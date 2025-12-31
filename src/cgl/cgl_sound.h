
#ifndef CGL_SOUND_H
#define CGL_SOUND_H

#include <SDL.h>

#include "cgl_context.h"

typedef struct CGL_Sound CGL_Sound;

CGL_Sound *CGL_LoadSound(const char *path, SDL_AudioDeviceID dev);

void CGL_SoundPlay(CGL_Sound *sound, CGL_Context *ctx);

void CGL_DestroySound(CGL_Sound *sound);

#endif
