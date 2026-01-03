
#include "pac_man.h"

#include <limits.h>

#include "../resources.h"
#include "../game.h"

#define PAC_MAN_GHOST_COUNT (4)

#define PAC_MAN_GHOST_BLINKY (0)
#define PAC_MAN_GHOST_PINKY  (1)
#define PAC_MAN_GHOST_INKY   (2)
#define PAC_MAN_GHOST_CLYDE  (3)

#define PAC_MAN_LEVEL_X (0)
#define PAC_MAN_LEVEL_Y (20)

typedef struct PacManChar PacManChar;
typedef struct PacManGhost PacManGhost;
typedef struct PacManPlayer PacManPlayer;
typedef struct PacManScreenData PacManScreenData;

typedef void (*PacManCharUpdateFunc)(PacManChar *ch, CGL_Context *ctx, PacManScreenData *data);
typedef void (*PacManCharRenderFunc)(CGL_Context *ctx, PacManChar *ch, PacManScreenData *data);

typedef void (*PacManGhostTargetFunc)(PacManGhost *ghost, CGL_Context *ctx, PacManScreenData *data, Vector2I *target);

typedef enum {
  START,
  START_DELAY,
  PLAY,
  DEATH,
  GAME_OVER
} PacManGameState;

typedef enum {
  JAIL,
  JAIL_CENTER,
  JAIL_EXIT,
  DISPERSE,
  NORMAL,
  VULNERABLE,
  EYEBALLS
} PacManGhostState;

#define PAC_MAN_DIRECTION_COUNT ((DIRECTION_MAX - DIRECTION_MIN) + 1)

typedef enum {
  DIRECTION_MIN = 0,
  UP = DIRECTION_MIN,
  LEFT,
  DOWN,
  RIGHT,
  DIRECTION_MAX = RIGHT
} PacManDirection;

struct PacManChar {
  PacManCharUpdateFunc updateFunc;
  PacManCharRenderFunc renderFunc;
  uint16_t speed;     // Speed mask is 1 bit inside a byte, that shifts left every frame
  uint16_t speedMask; // If speed masked with speedMask is 1, then the character will move.
  bool dead;
  Vector2I deadTarget;
  Vector2I pos;
  Vector2I tilePos;
  PacManDirection dir;
  bool canMove[PAC_MAN_DIRECTION_COUNT];
  CGL_Animation *anims[PAC_MAN_DIRECTION_COUNT];
  CGL_Animation *deadAnims[PAC_MAN_DIRECTION_COUNT];
};

#define PAC_MAN_GHOST_JAIL_CENTER_TARGET ((Vector2I){.x = 104, .y = 112})
#define PAC_MAN_GHOST_JAIL_EXIT_TARGET   ((Vector2I){.x = 104, .y = 88})

struct PacManGhost {
  PacManChar charData;
  PacManGhostTargetFunc targetFunc;
  PacManGhostState state;
  int jailTimer;
};

struct PacManPlayer {
  PacManChar charData;
  int lives;
};

struct PacManScreenData {
  PacManLevel *lvl;
  PacManGameState state;
  int timer;
  PacManPlayer plyr;
  PacManGhost ghosts[PAC_MAN_GHOST_COUNT];
};

PacManDirection GetOppositeDirection(PacManDirection dir)
{
  switch(dir)
  {
    case UP:    return DOWN;
    case LEFT:  return RIGHT;
    case DOWN:  return UP;
    case RIGHT: return LEFT;
    default:    return UP;
  }
}

void GetTileInDirection(PacManDirection dir, const Vector2I *src, Vector2I *dst)
{
  switch(dir)
  {
    case UP:    *dst = (Vector2I){.x = src->x,   .y = src->y-1}; break;
    case LEFT:  *dst = (Vector2I){.x = src->x-1, .y = src->y};   break;
    case DOWN:  *dst = (Vector2I){.x = src->x,   .y = src->y+1}; break;
    case RIGHT: *dst = (Vector2I){.x = src->x+1, .y = src->y};   break;
  }
}

void PlayerUpdate(PacManChar *ch, CGL_Context *ctx, PacManScreenData *data)
{
  PacManMazeCellType curCell = PacManLevelGetCellAt(data->lvl, ch->tilePos.x, ch->tilePos.y);
  if(curCell == DOT || curCell == SUPER_DOT)
    PacManLevelSetCellAt(data->lvl, ch->tilePos.x, ch->tilePos.y, EMPTY);

  if(CGL_ContextGetInput(ctx, CGL_INPUT_UP))
  {
    if(ch->canMove[UP])
      ch->dir = UP;
  }
  else if(CGL_ContextGetInput(ctx, CGL_INPUT_LEFT))
  {
    if(ch->canMove[LEFT])
      ch->dir = LEFT;
  }
  else if(CGL_ContextGetInput(ctx, CGL_INPUT_DOWN))
  {
    if(ch->canMove[DOWN])
      ch->dir = DOWN;
  }
  else if(CGL_ContextGetInput(ctx, CGL_INPUT_RIGHT))
  {
    if(ch->canMove[RIGHT])
      ch->dir = RIGHT;
  }
}

void GhostUpdate(PacManChar *ch, CGL_Context *ctx, PacManScreenData *data)
{
  PacManGhost *ghost = NULL;

  //FIX ME: This is not a good way to do this, but will work for now.
  //Might ditch the target callback altogether, seems overkill
  for(int i = 0; i < PAC_MAN_GHOST_COUNT && ghost == NULL; i++)
  {
    if(ch == &(data->ghosts[i].charData))
      ghost = &(data->ghosts[i]);
  }

  if(ghost == NULL)
  {
    CGL_LogWarning("GhostUpdate was called, but ghost wasn't found.");
    return;
  }

  PacManGhostState prevState = ghost->state;

  if(ghost->state == JAIL)
  {
    ghost->jailTimer--;
    if(ghost->jailTimer <= 0)
    {
      ch->deadTarget = PAC_MAN_GHOST_JAIL_CENTER_TARGET;
      ghost->state = JAIL_CENTER;
    }
  }
  else if(ghost->state == JAIL_CENTER)
  {
    if(IsEqualVector2I(&(ch->pos), &(ch->deadTarget)))
    {
      ch->deadTarget = PAC_MAN_GHOST_JAIL_EXIT_TARGET;
      ghost->state = JAIL_EXIT;
    }
  }
  else if(ghost->state == JAIL_EXIT)
  {
    if(IsEqualVector2I(&(ch->pos), &(ch->deadTarget)))
    {
      ch->dead = false;
      ghost->state = NORMAL;
    }
  }
  else if(ghost->state == NORMAL)
  {
    Vector2I target;
    Vector2I nextTile;
    PacManDirection minDir = DIRECTION_MIN;
    int minDist = INT_MAX;

    ghost->targetFunc(ghost, ctx, data, &target);

    for(int i = DIRECTION_MIN; i <= DIRECTION_MAX; i++)
    {
      if(ch->canMove[i] && (PacManDirection)i != GetOppositeDirection(ch->dir))
      {
        GetTileInDirection(i, &(ch->tilePos), &nextTile);
        int dist = DistanceSquaredVector2I(&target, &nextTile);

        if((dist == minDist && rand()%2 == 0) || dist < minDist)
        {
          minDist = dist;
          minDir = i;
        }
      }
    }

    ch->dir = minDir;
  }

  if(ghost->state != prevState)
    CGL_LogInfo("Ghost changed from %d to %d", prevState, ghost->state);
}

void GetBlinkyTarget(PacManGhost *ghost, CGL_Context *ctx, PacManScreenData *data, Vector2I *target)
{
  *target = data->plyr.charData.tilePos;
}

void GetPinkyTarget(PacManGhost *ghost, CGL_Context *ctx, PacManScreenData *data, Vector2I *target)
{
  switch(data->plyr.charData.dir)
  {
    case UP:    *target = (Vector2I){.x = data->plyr.charData.tilePos.x,     .y = data->plyr.charData.tilePos.y - 4}; break;
    case LEFT:  *target = (Vector2I){.x = data->plyr.charData.tilePos.x - 4, .y = data->plyr.charData.tilePos.y    }; break;
    case DOWN:  *target = (Vector2I){.x = data->plyr.charData.tilePos.x,     .y = data->plyr.charData.tilePos.y + 4}; break;
    case RIGHT: *target = (Vector2I){.x = data->plyr.charData.tilePos.x + 4, .y = data->plyr.charData.tilePos.y    }; break;
  }
}

void GetInkyTarget(PacManGhost *ghost, CGL_Context *ctx, PacManScreenData *data, Vector2I *target)
{
  Vector2I pivot;
  switch(data->plyr.charData.dir)
  {
    case UP:    pivot = (Vector2I){.x = data->plyr.charData.tilePos.x,     .y = data->plyr.charData.tilePos.y - 2}; break;
    case LEFT:  pivot = (Vector2I){.x = data->plyr.charData.tilePos.x - 2, .y = data->plyr.charData.tilePos.y    }; break;
    case DOWN:  pivot = (Vector2I){.x = data->plyr.charData.tilePos.x,     .y = data->plyr.charData.tilePos.y + 2}; break;
    case RIGHT: pivot = (Vector2I){.x = data->plyr.charData.tilePos.x + 2, .y = data->plyr.charData.tilePos.y    }; break;
    default:    return;
  }

  int dx = pivot.x - data->ghosts[PAC_MAN_GHOST_BLINKY].charData.tilePos.x;
  int dy = pivot.y - data->ghosts[PAC_MAN_GHOST_BLINKY].charData.tilePos.y;

  *target = (Vector2I){
    .x = data->ghosts[PAC_MAN_GHOST_BLINKY].charData.tilePos.x + 2*dx,
    .y = data->ghosts[PAC_MAN_GHOST_BLINKY].charData.tilePos.y + 2*dy
  };
}

void GetClydeTarget(PacManGhost *ghost, CGL_Context *ctx, PacManScreenData *data, Vector2I *target)
{
  int dist = DistanceSquaredVector2I(&(ghost->charData.tilePos), &(data->plyr.charData.tilePos));
  if(dist > 64)
    *target = data->plyr.charData.tilePos;
  else
    *target = (Vector2I){.x = 1, .y = PAC_MAN_MAZE_HEIGHT-2};
}

CGL_Animation* GetCharAnimation(const PacManChar *ch)
{
  if(ch->dead)
    return ch->deadAnims[ch->dir];
  return ch->anims[ch->dir];
}

void UpdateChar(PacManChar *ch, CGL_Context *ctx, PacManScreenData *data)
{
  if(ch->dead)
  {
    //FIX ME: hacky solution to ghost anim/deadAnim issue.
    CGL_AnimationUpdate(ch->anims[ch->dir]);
    CGL_AnimationUpdate(ch->deadAnims[ch->dir]);

    ch->updateFunc(ch, ctx, data);

    if(!IsEqualVector2I(&(ch->pos), &(ch->deadTarget)))
    {
      Vector2I diff;
      SubtractVector2I(&(ch->deadTarget), &(ch->pos), &diff);
      AbsVector2I(&diff, &diff);

      if(diff.x > diff.y && diff.x > 0)
      {
        if(ch->pos.x > ch->deadTarget.x)
          ch->pos.x--;
        else
          ch->pos.x++;
      }
      else
      {
        if(ch->pos.y > ch->deadTarget.y)
          ch->pos.y--;
        else
          ch->pos.y++;
      }
    }
    return;
  }

  if(ch->pos.x % 8 == 0 && ch->pos.y % 8 == 0)
  {
    ch->tilePos = (Vector2I){
      .x = ch->pos.x / 8,
      .y = ch->pos.y / 8
    };

    ch->canMove[UP]    = PacManLevelGetCellAt(data->lvl, ch->tilePos.x,   ch->tilePos.y-1) != WALL;
    ch->canMove[LEFT]  = PacManLevelGetCellAt(data->lvl, ch->tilePos.x-1, ch->tilePos.y)   != WALL;
    ch->canMove[DOWN]  = PacManLevelGetCellAt(data->lvl, ch->tilePos.x,   ch->tilePos.y+1) != WALL;
    ch->canMove[RIGHT] = PacManLevelGetCellAt(data->lvl, ch->tilePos.x+1, ch->tilePos.y)   != WALL;

    ch->updateFunc(ch, ctx, data);
  }

  if(ch->canMove[ch->dir])
  {
    CGL_AnimationUpdate(GetCharAnimation(ch));

    if(ch->speed & ch->speedMask)
    {
      switch(ch->dir)
      {
        case UP:
          ch->pos.y--;
          if(ch->pos.y < 0)
            ch->pos.y = PAC_MAN_LEVEL_HEIGHT-1;
          break;

        case LEFT:
          ch->pos.x--;
          if(ch->pos.x < 0)
            ch->pos.x = PAC_MAN_LEVEL_WIDTH-1;
          break;

        case DOWN:
          ch->pos.y++;
          if(ch->pos.y >= PAC_MAN_LEVEL_HEIGHT)
            ch->pos.y = 0;
          break;

        case RIGHT:
          ch->pos.x++;
          if(ch->pos.x >= PAC_MAN_LEVEL_WIDTH)
            ch->pos.x = 0;
          break;
      }
    }
  }

  if(ch->speedMask == 0x8000)
    ch->speedMask = 1;
  else
    ch->speedMask = (ch->speedMask << 1);
}

void PlayerRender(CGL_Context *ctx, PacManChar *ch, PacManScreenData *data)
{
  CGL_DrawAnimation(ctx, GetCharAnimation(ch), ch->pos.x + PAC_MAN_LEVEL_X - 4, ch->pos.y + PAC_MAN_LEVEL_Y - 4, 16, 16);
}

void GhostRender(CGL_Context *ctx, PacManChar *ch, PacManScreenData *data)
{
  //FIX ME: This is not a good way to do this, but will work for now.
  for(int i = 0; i < PAC_MAN_GHOST_COUNT; i++)
  {
    if(ch == &(data->ghosts[i].charData))
    {
      PacManGhost *ghost = &(data->ghosts[i]);
      CGL_Animation *anim;

      if(ghost->state == EYEBALLS)
        anim = ch->deadAnims[ch->dir];
      else
        anim = ch->anims[ch->dir];

      CGL_DrawAnimation(ctx, anim, ch->pos.x + PAC_MAN_LEVEL_X - 4, ch->pos.y + PAC_MAN_LEVEL_Y - 4, 16, 16);
      return;
    }
  }
}

void RenderChar(CGL_Context *ctx, PacManChar *ch, PacManScreenData *data)
{
  ch->renderFunc(ctx, ch, data);
}

void InitGhostAnims(CGL_SpriteSheet *sheet, PacManChar *ch, int row)
{
  CGL_SpriteSheetGetSpriteAt(sheet, 0, row, CGL_AnimationGetFrame(ch->anims[RIGHT], 0));
  CGL_SpriteSheetGetSpriteAt(sheet, 1, row, CGL_AnimationGetFrame(ch->anims[RIGHT], 1));
  CGL_SpriteSheetGetSpriteAt(sheet, 2, row, CGL_AnimationGetFrame(ch->anims[LEFT], 0));
  CGL_SpriteSheetGetSpriteAt(sheet, 3, row, CGL_AnimationGetFrame(ch->anims[LEFT], 1));
  CGL_SpriteSheetGetSpriteAt(sheet, 4, row, CGL_AnimationGetFrame(ch->anims[UP], 0));
  CGL_SpriteSheetGetSpriteAt(sheet, 5, row, CGL_AnimationGetFrame(ch->anims[UP], 1));
  CGL_SpriteSheetGetSpriteAt(sheet, 6, row, CGL_AnimationGetFrame(ch->anims[DOWN], 0));
  CGL_SpriteSheetGetSpriteAt(sheet, 7, row, CGL_AnimationGetFrame(ch->anims[DOWN], 1));

  CGL_SpriteSheetGetSpriteAt(sheet, 4, 4, CGL_AnimationGetFrame(ch->deadAnims[RIGHT], 0));
  CGL_SpriteSheetGetSpriteAt(sheet, 5, 4, CGL_AnimationGetFrame(ch->deadAnims[LEFT], 0));
  CGL_SpriteSheetGetSpriteAt(sheet, 6, 4, CGL_AnimationGetFrame(ch->deadAnims[UP], 0));
  CGL_SpriteSheetGetSpriteAt(sheet, 7, 4, CGL_AnimationGetFrame(ch->deadAnims[DOWN], 0));
}

void PacManScreenSetLevel(CGL_Screen *screen, PacManLevel *lvl, CGL_Context *ctx)
{
  PacManScreenData *data = CGL_ScreenGetData(screen);
  if(data == NULL)
    return;
  
  data->lvl = lvl;
  if(lvl == NULL)
    return;

  data->state = START;
  data->timer = 0;

  //Reset player
  PacManLevelGetSpawn(lvl, &(data->plyr.charData.pos));
  data->plyr.charData.dir = LEFT;
  data->plyr.charData.speedMask = 1;
  data->plyr.lives = 5;

  //reset ghosts
  for(int i = 0; i < PAC_MAN_GHOST_COUNT; i++)
  {
    data->ghosts[i].charData.dir = UP;
    //FIX ME: need to pull this from level
    data->ghosts[i].charData.pos = (Vector2I){
      .x = 76 + 16*i,
      .y = 112
    };
    data->ghosts[i].charData.tilePos = (Vector2I){
      .x = 0,
      .y = 0
    };
    data->ghosts[i].charData.dead = true;
    data->ghosts[i].charData.deadTarget = data->ghosts[i].charData.pos;

    data->ghosts[i].charData.speedMask = 1;
    data->ghosts[i].state = JAIL;
    data->ghosts[i].jailTimer = 120*i;
  }

  data->ghosts[PAC_MAN_GHOST_BLINKY].charData.pos = (Vector2I){
    .x = 104,
    .y = 88
  };
  data->ghosts[PAC_MAN_GHOST_BLINKY].charData.dead = false;
  data->ghosts[PAC_MAN_GHOST_BLINKY].state = NORMAL;
}

int PacManScreenInit(CGL_Screen *screen, CGL_Context *ctx)
{
  PacManScreenData *data = (PacManScreenData*)malloc(sizeof(PacManScreenData));
  if(data == NULL)
    return -1;

  data->lvl = NULL;
  data->state = START;
  data->timer = 0;

  data->plyr.charData.updateFunc = PlayerUpdate;
  data->plyr.charData.renderFunc = PlayerRender;
  data->plyr.charData.speed = 0xFFFF;
  data->plyr.charData.dead = false;

  for(int i = 0; i < PAC_MAN_DIRECTION_COUNT; i++)
  {
    data->plyr.charData.anims[i] = CGL_InitAnimation(3, 4, true);
    data->plyr.charData.canMove[i] = false;
    if(data->plyr.charData.anims[i] == NULL)
      return -1;
    data->plyr.charData.deadAnims[i] = NULL;

    for(int g = 0; g < PAC_MAN_GHOST_COUNT; g++)
    {
      data->ghosts[g].charData.canMove[i] = false;

      data->ghosts[g].charData.anims[i] = CGL_InitAnimation(2, 5, true);
      if(data->ghosts[g].charData.anims[i] == NULL)
        return -1;

      data->ghosts[g].charData.deadAnims[i] = CGL_InitAnimation(1, 1000, true);
      if(data->ghosts[g].charData.deadAnims[i] == NULL)
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

  for(int i = 0; i < PAC_MAN_GHOST_COUNT; i++)
  {
    data->ghosts[i].charData.updateFunc = GhostUpdate;
    data->ghosts[i].charData.renderFunc = GhostRender;
    data->ghosts[i].charData.speed = 0xFFFE;
  }

  //BLINKY
  InitGhostAnims(ghostSheet, &(data->ghosts[PAC_MAN_GHOST_BLINKY].charData), 0);
  data->ghosts[PAC_MAN_GHOST_BLINKY].targetFunc = GetBlinkyTarget;

  //PINKY
  InitGhostAnims(ghostSheet, &(data->ghosts[PAC_MAN_GHOST_PINKY].charData), 1);
  data->ghosts[PAC_MAN_GHOST_PINKY].targetFunc = GetPinkyTarget;

  //INKY
  InitGhostAnims(ghostSheet, &(data->ghosts[PAC_MAN_GHOST_INKY].charData), 2);
  data->ghosts[PAC_MAN_GHOST_INKY].targetFunc = GetInkyTarget;

  //CLYDE
  InitGhostAnims(ghostSheet, &(data->ghosts[PAC_MAN_GHOST_CLYDE].charData), 3);
  data->ghosts[PAC_MAN_GHOST_CLYDE].targetFunc = GetClydeTarget;

  CGL_ScreenSetData(screen, data);
  return 0;
}

void PacManScreenUpdate(CGL_Screen *screen, CGL_Context *ctx)
{
  PacManScreenData *data = CGL_ScreenGetData(screen);
  if(data->lvl == NULL)
    return;

  if(data->state == START)
  {
    CGL_Sound *intro = ResourcesGetSound(SOUND_PAC_MAN_START);
    CGL_SoundPlay(intro, ctx);
    data->state = START_DELAY;
  }
  else if(data->state == START_DELAY)
  {
    data->timer++;
    if(data->timer == 240)
      data->state = PLAY;
  }
  else if(data->state == PLAY)
  {
    UpdateChar(&(data->plyr.charData), ctx, data);
    for(int i = 0; i < PAC_MAN_GHOST_COUNT; i++)
      UpdateChar(&(data->ghosts[i].charData), ctx, data);
  }
}

void PacManScreenRender(CGL_Screen *screen, CGL_Context *ctx)
{
  PacManScreenData *data = CGL_ScreenGetData(screen);

  if(data->lvl == NULL)
    return;

  PacManLevelRender(ctx, data->lvl, PAC_MAN_LEVEL_X, PAC_MAN_LEVEL_Y);
  RenderChar(ctx, &(data->plyr.charData), data);
  for(int i = 0; i < PAC_MAN_GHOST_COUNT; i++)
    RenderChar(ctx, &(data->ghosts[i].charData), data);
}

void PacManScreenDestroy(CGL_Screen *screen)
{
  PacManScreenData *data = CGL_ScreenGetData(screen);
  if(data == NULL)
    return;
  free(data);
}

