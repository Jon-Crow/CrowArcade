
#include "pac_level.h"

struct PacManLevel {
  CGL_TextureRegion* mazeTx;
  PacManMazeCellType maze[PAC_MAN_MAZE_SIZE];
};

//FIX ME: Not finished
PacManLevel* CreatePacManLevel(int lvlCol, int lvlRow)
{
  CGL_TextureRegion* levelsTx = ResourcesGetTexture(TEXTURE_PAC_MAN_LEVELS);
  if(levelsTx == NULL)
    return NULL;

  PacManLevel* level = (PacManLevel*)malloc(sizeof(PacManLevel));
  if(level == NULL)
    return NULL;

  return level;
}

void DestroyPacManLevel(PacManLevel* level)
{
  if(level == NULL)
    return;
  free(level);
}
