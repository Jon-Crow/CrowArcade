
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

CGL_Texture* CGL_TextureRegionGetTexture(CGL_TextureRegion *reg);
void CGL_TextureRegionSetTexture(CGL_TextureRegion *reg, CGL_Texture *tx);

int CGL_TextureRegionGetX(CGL_TextureRegion *reg);
void CGL_TextureRegionSetX(CGL_TextureRegion *reg, int x);
int CGL_TextureRegionGetY(CGL_TextureRegion *reg);
void CGL_TextureRegionSetY(CGL_TextureRegion *reg, int y);
void CGL_TextureRegionGetPos(CGL_TextureRegion *reg, int *x, int *y);
void CGL_TextureRegionSetPos(CGL_TextureRegion *reg, int x, int y);

int CGL_TextureRegionGetWidth(CGL_TextureRegion *reg);
void CGL_TextureRegionSetWidth(CGL_TextureRegion *reg, int width);
int CGL_TextureRegionGetHeight(CGL_TextureRegion *reg);
void CGL_TextureRegionSetHeight(CGL_TextureRegion *reg, int height);
void CGL_TextureRegionGetSize(CGL_TextureRegion *reg, int *width, int *height);
void CGL_TextureRegionSetSize(CGL_TextureRegion *reg, int width, int height);

void CGL_TextureRegionGetBounds(CGL_TextureRegion *reg, int *x, int *y, int *width, int *height);
void CGL_TextureRegionSetBounds(CGL_TextureRegion *reg, int x, int y, int width, int height);
void CGL_TextureRegionGetRect(CGL_TextureRegion *reg, SDL_Rect *bounds);
void CGL_TextureRegionSetRect(CGL_TextureRegion *reg, const SDL_Rect *bounds);

void CGL_DestroyTexture(CGL_Texture *tx);
//NOTE: This does not destroy the texture!
void CGL_DestroyTextureRegion(CGL_TextureRegion *reg);

#endif