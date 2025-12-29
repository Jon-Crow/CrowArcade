
#include "pac_man.h"

#include "../game.h"

#define PAC_MAN_GHOST_COUNT (4)

#define PAC_MAN_GHOST_BLINKY (0)
#define PAC_MAN_GHOST_PINKY  (1)
#define PAC_MAN_GHOST_INKY   (2)
#define PAC_MAN_GHOST_CLYDE  (3)

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

typedef struct PacManGhost PacManGhost;

struct PacManGhost {
  PacManGhostState state;
  Vector2I pos;
  PacManGhostAIUpdate aiUpdate;
  int jailTimer;
};

typedef struct PacManPlayer PacManPlayer;

struct PacManPlayer {
  Vector2I pos;
};

typedef struct PacManScreenData PacManScreenData;

struct PacManScreenData {
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

int PacManScreenInit(CGL_Screen *screen)
{
  PacManScreenData *data = (PacManScreenData*)malloc(sizeof(PacManScreenData));
  if(data == NULL)
    return -1;

  data->state = START_DELAY;
  data->timer = 0;
  
  //PLAYER
  data->plyr.pos = (Vector2I){
    .x = 0,
    .y = 0
  };
  
  //BLINKY
  data->ghosts[PAC_MAN_GHOST_BLINKY].state = NORMAL;
  data->ghosts[PAC_MAN_GHOST_BLINKY].pos = (Vector2I){
    .x = 0,
    .y = 0
  };
  data->ghosts[PAC_MAN_GHOST_BLINKY].aiUpdate = BlinkyUpdate;
  data->ghosts[PAC_MAN_GHOST_BLINKY].jailTimer = 0;

  //PINKY
  data->ghosts[PAC_MAN_GHOST_PINKY].state = JAIL;
  data->ghosts[PAC_MAN_GHOST_PINKY].pos = (Vector2I){
    .x = 0,
    .y = 0
  };
  data->ghosts[PAC_MAN_GHOST_PINKY].aiUpdate = PinkyUpdate;
  data->ghosts[PAC_MAN_GHOST_PINKY].jailTimer = 5;

  //INKY
  data->ghosts[PAC_MAN_GHOST_INKY].state = JAIL;
  data->ghosts[PAC_MAN_GHOST_INKY].pos = (Vector2I){
    .x = 0,
    .y = 0
  };
  data->ghosts[PAC_MAN_GHOST_INKY].aiUpdate = InkyUpdate;
  data->ghosts[PAC_MAN_GHOST_INKY].jailTimer = 10;

  //CLYDE
  data->ghosts[PAC_MAN_GHOST_CLYDE].state = JAIL;
  data->ghosts[PAC_MAN_GHOST_CLYDE].pos = (Vector2I){
    .x = 0,
    .y = 0
  };
  data->ghosts[PAC_MAN_GHOST_CLYDE].aiUpdate = ClydeUpdate;
  data->ghosts[PAC_MAN_GHOST_CLYDE].jailTimer = 15;

  CGL_ScreenSetData(screen, data);
  return 0;
}

void PacManScreenUpdateStartDelay(CGL_Screen *screen, CGL_Context *ctx)
{

}

void PacManScreenUpdatePlay(CGL_Screen *screen, CGL_Context *ctx)
{

}

void PacManScreenUpdateDeath(CGL_Screen *screen, CGL_Context *ctx)
{

}

void PacManScreenUpdateGameOver(CGL_Screen *screen, CGL_Context *ctx)
{

}

void PacManScreenUpdate(CGL_Screen *screen, CGL_Context *ctx)
{
  PacManScreenData *data = CGL_ScreenGetData(screen);
  switch(data->state)
  {
    case START_DELAY:
      PacManScreenUpdateStartDelay();
      break;
    case PLAY:
      PacManScreenUpdatePlay();
      break;
    case DEATH:
      PacManScreenUpdateDeath();
      break;
    case GAME_OVER:
      PacManScreenUpdateGameOver();
      break;
    default:
      printf("ERROR: Game in unknown state!\n");
  }
}

void PacManScreenRender(CGL_Screen *screen, CGL_Context *ctx)
{

}

void PacManScreenDestroy(CGL_Screen *screen)
{
  PacManScreenData *data = CGL_ScreenGetData(screen);
  if(data == NULL)
    return;
  free(data);
}

