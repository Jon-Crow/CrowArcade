
#include "pac_level.h"

#include "cJSON.h"

#include "../resources.h"

struct PacManLevel {
  CGL_TextureRegion* mazeTx;
  PacManMazeCellType maze[PAC_MAN_MAZE_SIZE];
  SDL_Color dotColor;
};

//FIX ME: Not finished
PacManLevel* CreatePacManLevel(int lvlCol, int lvlRow)
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

  level->dotColor = (SDL_Color){.r = 255, .g = 255, .b = 255, .a = 255};

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

bool PacManLevelParseMaze(cJSON *lvlJson, PacManMazeCellType *maze)
{
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
            maze[i] = cell->valueint;
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
  if(!PacManLevelParseMaze(json, maze))
  {
    printf("ERROR: Level json key missing or invalid: %s\n", PAC_MAN_JSON_KEY_MAZE);
    return NULL;
  }

  cJSON_Delete(json);

  printf("Creating Pac-Man level with map texture at %d, %d\n", lvlCol, lvlRow);
  PacManLevel *lvl = CreatePacManLevel(lvlCol, lvlRow);
  if(lvl == NULL)
    return NULL;

  for(int i = 0; i < PAC_MAN_MAZE_SIZE; i++)
    lvl->maze[i] = maze[i];

  return lvl;
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
