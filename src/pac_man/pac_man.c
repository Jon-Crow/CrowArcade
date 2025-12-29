
#include "pac_man.h"

#include "../resources.h"
#include "../game.h"

#define PAC_MAN_GHOST_COUNT (4)

#define PAC_MAN_GHOST_BLINKY (0)
#define PAC_MAN_GHOST_PINKY  (1)
#define PAC_MAN_GHOST_INKY   (2)
#define PAC_MAN_GHOST_CLYDE  (3)

#define PAC_MAN_LEVEL_X (0)
#define PAC_MAN_LEVEL_Y (20)

typedef enum {
  START_DELAY,
  PLAY,
  DEATH,
  GAME_OVER
} PacManGameState;

typedef void (*PacManGhostAIUpdate)();

typedef enum {
  JAIL,
  NORMAL,
  VULNERABLE,
  EYEBALLS
} PacManGhostState;

typedef enum {
  UP    = 0,
  LEFT  = 1,
  DOWN  = 2,
  RIGHT = 3
} PacManDirection;

typedef struct PacManChar PacManChar;

struct PacManChar {
  Vector2I pos;
  PacManDirection dir;
  CGL_Animation *anims[4];
};

typedef struct PacManGhost PacManGhost;

struct PacManGhost {
  PacManChar charData;
  PacManGhostState state;
  PacManGhostAIUpdate aiUpdate;
  int jailTimer;
};

typedef struct PacManPlayer PacManPlayer;

struct PacManPlayer {
  PacManChar charData;
  int lives;
};

typedef struct PacManScreenData PacManScreenData;

struct PacManScreenData {
  PacManLevel *lvl;
  PacManGameState state;
  int timer;
  PacManPlayer plyr;
  PacManGhost ghosts[PAC_MAN_GHOST_COUNT];
};

void BlinkyUpdate()
{}

void PinkyUpdate()
{}

void InkyUpdate()
{}

void ClydeUpdate()
{}

void PacManScreenSetLevel(CGL_Screen *screen, PacManLevel *lvl)
{
  PacManScreenData *data = CGL_ScreenGetData(screen);
  if(data == NULL)
    return;
  
  data->lvl = lvl;
  if(lvl == NULL)
    return;

  data->state = START_DELAY;
  data->timer = 0;

  //PLAYER
  PacManLevelGetSpawn(lvl, &(data->plyr.charData.pos));
  data->plyr.charData.dir = LEFT;
  data->plyr.lives = 5;
}

CGL_Animation* GetCharAnimation(const PacManChar *ch)
{
  return ch->anims[ch->dir];
}

bool IsWallNextToChar(PacManChar *ch, PacManDirection dir, PacManLevel *lvl)
{
  //This is probably overly convoluted, but it is meant to get the proper edge
  //of the player's cell, depending on query and movement direction. Might
  //break if the direction opposite of movement is checked
  int col = (ch->dir == LEFT || dir == LEFT) ? (ch->pos.x + 7)/8 : ch->pos.x/8;
  int row = (ch->dir == UP   || dir == UP  ) ? (ch->pos.y + 7)/8 : ch->pos.y/8;

  switch(dir)
  {
    case UP:    return PacManLevelGetCellAt(lvl, col,   row-1) == WALL;
    case LEFT:  return PacManLevelGetCellAt(lvl, col-1, row)   == WALL;
    case DOWN:  return PacManLevelGetCellAt(lvl, col,   row+1) == WALL;
    case RIGHT: return PacManLevelGetCellAt(lvl, col+1, row)   == WALL;
    default: return true;
  }
}

void UpdateChar(PacManChar *ch, PacManLevel *lvl)
{
  CGL_AnimationUpdate(GetCharAnimation(ch));

  if(!IsWallNextToChar(ch, ch->dir, lvl))
  {
    switch(ch->dir)
    {
      case UP:    ch->pos.y--; break;
      case LEFT:  ch->pos.x--; break;
      case DOWN:  ch->pos.y++; break;
      case RIGHT: ch->pos.x++; break;
    }
  }
}

void RenderChar(CGL_Context *ctx, PacManChar *ch)
{
  CGL_DrawAnimation(ctx, GetCharAnimation(ch), ch->pos.x + PAC_MAN_LEVEL_X - 4, ch->pos.y + PAC_MAN_LEVEL_Y - 4, 16, 16);
  //SDL_Color clr = {
  //  .r = 255,
  //  .g = 0,
  //  .b = 0,
  //  .a = 255
  //};
  //CGL_GraphicsDrawFilledCircle(ctx, ch->pos.x + PAC_MAN_LEVEL_X, ch->pos.y + PAC_MAN_LEVEL_Y, 8, &clr);
  //CGL_GraphicsDrawFilledRect(ctx, ch->pos.x + PAC_MAN_LEVEL_X, ch->pos.y + PAC_MAN_LEVEL_Y, 8, 8, &clr);
}

int PacManScreenInit(CGL_Screen *screen)
{
  PacManScreenData *data = (PacManScreenData*)malloc(sizeof(PacManScreenData));
  if(data == NULL)
    return -1;

  data->lvl = NULL;
  data->state = START_DELAY;
  data->timer = 0;

  for(int i = 0; i < 4; i++)
  {
    data->plyr.charData.anims[i] = CGL_InitAnimation(3, 4, true);
    if(data->plyr.charData.anims[i] == NULL)
      return -1;

    for(int g = 0; g < PAC_MAN_GHOST_COUNT; g++)
    {
      data->ghosts[g].charData.anims[i] = CGL_InitAnimation(2, 5, true);
      if(data->ghosts[g].charData.anims[i] == NULL)
        return -1;
    }
  }

  CGL_SpriteSheet *plyrSheet = ResourcesGetSpriteSheet(SPRITE_SHEET_PAC_MAN_PLAYER);
  CGL_SpriteSheet *ghostSheet = ResourcesGetSpriteSheet(SPRITE_SHEET_PAC_MAN_GHOSTS);

  CGL_SpriteSheetGetSpriteAt(plyrSheet, 0, 2, CGL_AnimationGetFrame(data->plyr.charData.anims[UP], 0));
  CGL_SpriteSheetGetSpriteAt(plyrSheet, 1, 2, CGL_AnimationGetFrame(data->plyr.charData.anims[UP], 1));
  CGL_SpriteSheetGetSpriteAt(plyrSheet, 2, 2, CGL_AnimationGetFrame(data->plyr.charData.anims[UP], 2));
  CGL_SpriteSheetGetSpriteAt(plyrSheet, 0, 1, CGL_AnimationGetFrame(data->plyr.charData.anims[LEFT], 0));
  CGL_SpriteSheetGetSpriteAt(plyrSheet, 1, 1, CGL_AnimationGetFrame(data->plyr.charData.anims[LEFT], 1));
  CGL_SpriteSheetGetSpriteAt(plyrSheet, 2, 1, CGL_AnimationGetFrame(data->plyr.charData.anims[LEFT], 2));
  CGL_SpriteSheetGetSpriteAt(plyrSheet, 0, 3, CGL_AnimationGetFrame(data->plyr.charData.anims[DOWN], 0));
  CGL_SpriteSheetGetSpriteAt(plyrSheet, 1, 3, CGL_AnimationGetFrame(data->plyr.charData.anims[DOWN], 1));
  CGL_SpriteSheetGetSpriteAt(plyrSheet, 2, 3, CGL_AnimationGetFrame(data->plyr.charData.anims[DOWN], 2));
  CGL_SpriteSheetGetSpriteAt(plyrSheet, 0, 0, CGL_AnimationGetFrame(data->plyr.charData.anims[RIGHT], 0));
  CGL_SpriteSheetGetSpriteAt(plyrSheet, 1, 0, CGL_AnimationGetFrame(data->plyr.charData.anims[RIGHT], 1));
  CGL_SpriteSheetGetSpriteAt(plyrSheet, 2, 0, CGL_AnimationGetFrame(data->plyr.charData.anims[RIGHT], 2));

  CGL_ScreenSetData(screen, data);
  return 0;
}

void PacManScreenUpdate(CGL_Screen *screen, CGL_Context *ctx)
{
  PacManScreenData *data = CGL_ScreenGetData(screen);
  if(data->lvl == NULL)
    return;

  if(data->state == START_DELAY)
  {
    data->timer++;
    if(data->timer == 120)
      data->state = PLAY;
    return;
  }

  if(data->state == PLAY)
  {
    UpdateChar(&(data->plyr.charData), data->lvl);

    int col = (data->plyr.charData.pos.x + 4)/8;
    int row = (data->plyr.charData.pos.y + 4)/8;
    
    PacManMazeCellType curCell = PacManLevelGetCellAt(data->lvl, col, row);
    if(curCell == DOT || curCell == SUPER_DOT)
      PacManLevelSetCellAt(data->lvl, col, row, EMPTY);

    if(CGL_ContextGetInput(ctx, CGL_INPUT_UP))
    {
      if(!IsWallNextToChar(&(data->plyr.charData), UP, data->lvl))
        data->plyr.charData.dir = UP;
    }
    else if(CGL_ContextGetInput(ctx, CGL_INPUT_LEFT))
    {
      if(!IsWallNextToChar(&(data->plyr.charData), LEFT, data->lvl))
        data->plyr.charData.dir = LEFT;
    }
    else if(CGL_ContextGetInput(ctx, CGL_INPUT_DOWN))
    {
      if(!IsWallNextToChar(&(data->plyr.charData), DOWN, data->lvl))
        data->plyr.charData.dir = DOWN;
    }
    else if(CGL_ContextGetInput(ctx, CGL_INPUT_RIGHT))
    {
      if(!IsWallNextToChar(&(data->plyr.charData), RIGHT, data->lvl))
        data->plyr.charData.dir = RIGHT;
    }
  }
}

void PacManScreenRender(CGL_Screen *screen, CGL_Context *ctx)
{
  PacManScreenData *data = CGL_ScreenGetData(screen);

  if(data->lvl == NULL)
    return;

  PacManLevelRender(ctx, data->lvl, PAC_MAN_LEVEL_X, PAC_MAN_LEVEL_Y);
  RenderChar(ctx, &(data->plyr.charData));
}

void PacManScreenDestroy(CGL_Screen *screen)
{
  PacManScreenData *data = CGL_ScreenGetData(screen);
  if(data == NULL)
    return;
  free(data);
}

