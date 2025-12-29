
#include "pac_level.h"

#include "cJSON.h"

#include "../resources.h"

struct PacManLevel {
  CGL_TextureRegion* mazeTx;
  PacManMazeCellType maze[PAC_MAN_MAZE_SIZE];
  int dotCount;
  SDL_Color dotColor;
  Vector2I spawn;
};

//FIX ME: Not finished
PacManLevel* CreatePacManLevel(int lvlCol, int lvlRow, const SDL_Color *dotColor, const Vector2I *spawn)
{
  CGL_SpriteSheet *lvlSheet = ResourcesGetSpriteSheet(SPRITE_SHEET_PAC_MAN_LEVELS);
  if(!CGL_SpriteSheetIsInBounds(lvlSheet, lvlCol, lvlRow))
  {
    printf("Provided Pac-Man level texture index out of bounds.\n");
    printf("Requested: %d,%d\n", lvlCol, lvlRow);
    printf("Sheet size: %d,%d\n", CGL_SpriteSheetGetColumns(lvlSheet), CGL_SpriteSheetGetRows(lvlSheet));
    return NULL;
  }

  PacManLevel* level = (PacManLevel*)malloc(sizeof(PacManLevel));
  if(level == NULL)
    return NULL;
  
  level->mazeTx = CGL_InitTextureRegion();
  if(level->mazeTx == NULL)
  {
    free(level);
    return NULL;
  }

  CGL_SpriteSheetGetSpriteAt(lvlSheet, lvlCol, lvlRow, level->mazeTx);

  for(int i = 0; i < PAC_MAN_MAZE_SIZE; i++)
    level->maze[i] = EMPTY;
  level->dotCount = 0;

  if(dotColor == NULL)
    level->dotColor = (SDL_Color){.r = 255, .g = 255, .b = 255, .a = 255};
  else
    level->dotColor = *dotColor;

  level->spawn = *spawn;

  return level;
}

bool IsValidPacManMazeCellType(PacManMazeCellType val)
{
  switch(val)
  {
    case EMPTY:
    case WALL:
    case DOT:
    case SUPER_DOT:
      return true;
    default:
      return false;
  }
}

bool PacManLevelParseDotColor(cJSON *lvlJson, SDL_Color *clr)
{
  cJSON *clrJson = cJSON_GetObjectItemCaseSensitive(lvlJson, PAC_MAN_JSON_KEY_DOT_COLOR);
  if(cJSON_IsObject(clrJson))
  {
    cJSON *rJson = cJSON_GetObjectItemCaseSensitive(clrJson, PAC_MAN_JSON_KEY_DOT_COLOR_R);
    if(cJSON_IsNumber(rJson))
      clr->r = rJson->valueint;
    else
      return false;

    cJSON *gJson = cJSON_GetObjectItemCaseSensitive(clrJson, PAC_MAN_JSON_KEY_DOT_COLOR_G);
    if(cJSON_IsNumber(gJson))
      clr->g = gJson->valueint;
    else
      return false;

    cJSON *bJson = cJSON_GetObjectItemCaseSensitive(clrJson, PAC_MAN_JSON_KEY_DOT_COLOR_B);
    if(cJSON_IsNumber(bJson))
      clr->b = bJson->valueint;
    else
      return false;

    cJSON *aJson = cJSON_GetObjectItemCaseSensitive(clrJson, PAC_MAN_JSON_KEY_DOT_COLOR_A);
    if(cJSON_IsNumber(aJson))
      clr->a = aJson->valueint;
    else
      return false;
  }
  else
    return false;

  return true;
}

bool PacManLevelParseMaze(cJSON *lvlJson, PacManMazeCellType *maze, int *dotCount)
{
  *dotCount = 0;

  cJSON *mazeJson = cJSON_GetObjectItemCaseSensitive(lvlJson, PAC_MAN_JSON_KEY_MAZE);
  if(cJSON_IsArray(mazeJson))
  {
    int mazeLen = cJSON_GetArraySize(mazeJson);
    if(mazeLen == PAC_MAN_MAZE_SIZE)
    {
      for(int i = 0; i < PAC_MAN_MAZE_SIZE; i++)
      {
        cJSON *cell = cJSON_GetArrayItem(mazeJson, i);
        if(cJSON_IsNumber(cell))
        {
          if(IsValidPacManMazeCellType(cell->valueint))
          {
            maze[i] = cell->valueint;
            if(maze[i] == DOT || maze[i] == SUPER_DOT)
              (*dotCount)++;
          }
          else
            return false;
        }
        else
          return false;
      }
    }
    else
      return false;
  }
  else
    return false;

  return true;
}

PacManLevel* LoadPacManLevel(const char *jsonPath)
{
  cJSON *json = ResourcesParseJsonFile(jsonPath);
  if(json == NULL)
    return NULL;

  cJSON *lvlColJson = cJSON_GetObjectItemCaseSensitive(json, PAC_MAN_JSON_KEY_TX_COL);
  int lvlCol = -1;
  if(cJSON_IsNumber(lvlColJson))
    lvlCol = lvlColJson->valueint;
  else
    printf("ERROR: Level json key missing or invalid: %s\n", PAC_MAN_JSON_KEY_TX_COL);

  cJSON *lvlRowJson = cJSON_GetObjectItemCaseSensitive(json, PAC_MAN_JSON_KEY_TX_ROW);
  int lvlRow = -1;
  if(cJSON_IsNumber(lvlRowJson))
    lvlRow = lvlRowJson->valueint;
  else
    printf("ERROR: Level json key missing or invalid: %s\n", PAC_MAN_JSON_KEY_TX_ROW);

  PacManMazeCellType maze[PAC_MAN_MAZE_SIZE];
  int dotCount;
  if(!PacManLevelParseMaze(json, maze, &dotCount))
  {
    printf("ERROR: Level json key missing or invalid: %s\n", PAC_MAN_JSON_KEY_MAZE);
    cJSON_Delete(json);
    return NULL;
  }

  SDL_Color dotColor;
  if(!PacManLevelParseDotColor(json, &dotColor))
  {
    printf("ERROR: Level json key missing or invalid: %s\n", PAC_MAN_JSON_KEY_DOT_COLOR);
    cJSON_Delete(json);
    return NULL;
  }

  cJSON *spawnJson = cJSON_GetObjectItemCaseSensitive(json, PAC_MAN_JSON_KEY_SPAWN);
  Vector2I spawn;
  if(!ParseVector2I(spawnJson, &spawn))
  {
    printf("ERROR: Level json key missing or invalid: %s\n", PAC_MAN_JSON_KEY_SPAWN);
    cJSON_Delete(json);
    return NULL;
  }

  cJSON_Delete(json);

  printf("Creating Pac-Man level with map texture at %d, %d\n", lvlCol, lvlRow);
  PacManLevel *lvl = CreatePacManLevel(lvlCol, lvlRow, &dotColor, &spawn);
  if(lvl == NULL)
    return NULL;

  for(int i = 0; i < PAC_MAN_MAZE_SIZE; i++)
    lvl->maze[i] = maze[i];
  lvl->dotCount = dotCount;

  return lvl;
}

void PacManLevelGetSpawn(PacManLevel* level, Vector2I *v)
{
  *v = level->spawn;
}

PacManMazeCellType PacManLevelGetCellAt(PacManLevel *level, int col, int row)
{
  int idx = row*PAC_MAN_MAZE_WIDTH + col;
  if(idx < 0 || idx >= PAC_MAN_MAZE_SIZE)
    return WALL;
  return level->maze[idx];
}

void PacManLevelSetCellAt(PacManLevel *level, int col, int row, PacManMazeCellType cell)
{
  int idx = row*PAC_MAN_MAZE_WIDTH + col;
  if(idx < 0 || idx >= PAC_MAN_MAZE_SIZE)
    return;

  PacManMazeCellType prev = level->maze[idx];
  level->maze[idx] = cell;

  if((prev == DOT || prev == SUPER_DOT) && !(cell == DOT || cell == SUPER_DOT))
    level->dotCount--;
  else if(!(prev == DOT || prev == SUPER_DOT) && (cell == DOT || cell == SUPER_DOT))
    level->dotCount++;
}

int PacManLevelGetDotCount(PacManLevel *level)
{
  return level->dotCount;
}

void PacManLevelRender(CGL_Context *ctx, PacManLevel *lvl, int startX, int startY)
{
  CGL_DrawTextureRegion(ctx, lvl->mazeTx, startX, startY, -1, -1);
  int idx = 0;
  for(int y = 0; y < PAC_MAN_MAZE_HEIGHT; y++)
  {
    for(int x = 0; x < PAC_MAN_MAZE_WIDTH; x++)
    {
      switch(lvl->maze[idx])
      {
        case DOT:
          CGL_GraphicsDrawFilledRect(ctx, startX + x*8 + 3, startY + y*8 + 3, 2, 2, &lvl->dotColor);
          break;
        case SUPER_DOT:
          CGL_GraphicsDrawFilledCircle(ctx, startX + x*8 + 4, startY + y*8 + 4, 4, &lvl->dotColor);
          break;
        
        default:
      }
      idx++;
    }
  }
}

void DestroyPacManLevel(PacManLevel* level)
{
  if(level != NULL)
    free(level);
}
