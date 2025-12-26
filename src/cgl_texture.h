
#ifndef CGL_RESOURCES_H
#define CGL_RESOURCES_H

typedef struct CGL_Texture CGL_Texture;
typedef struct CGL_TextureRegion CGL_TextureRegion;

CGL_TextureRegion* CGL_CreateTextureRegion(CGL_Texture *tx, const SDL_Rect *rect);

CGL_Texture* CGL_LoadTexture(SDL_Renderer *rend, const char *path);

SDL_Texture* CGL_TextureGetImage(CGL_Texture *tx);
int CGL_TextureGetWidth(CGL_Texture *tx);
int CGL_TextureGetHeight(CGL_Texture *tx);
void CGL_TextureGetSize(CGL_Texture *tx, int *width, int *height);

void CGL_DestroyTexture(CGL_Texture *tx);
//NOTE: This does not destroy the texture!
void CGL_DestroyTextureRegion(CGL_TextureRegion *reg);

#endif