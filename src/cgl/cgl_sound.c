
#include "cgl_sound.h"

struct CGL_Sound {
  SDL_AudioSpec audioSpec;
  Uint32 length;
  Uint8 *buffer;
};

CGL_Sound *CGL_LoadSound(const char *path, SDL_AudioDeviceID dev)
{
  CGL_Sound *sound = (CGL_Sound*)malloc(sizeof(CGL_Sound));
  if(sound == NULL)
    return NULL;

  SDL_AudioSpec *spec = SDL_LoadWAV(path, &(sound->audioSpec), &(sound->buffer), &(sound->length));
  if(spec == NULL)
  {
    free(sound);
    return NULL;
  }

  return sound;
}

void CGL_SoundPlay(CGL_Sound *sound, CGL_Context *ctx)
{
  SDL_QueueAudio(CGL_ContextGetAudioDeviceID(ctx), sound->buffer, sound->length);
}

void CGL_DestroySound(CGL_Sound *sound)
{
  SDL_FreeWAV(sound->buffer);
  free(sound);
}
