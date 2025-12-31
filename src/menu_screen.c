
#include "menu_screen.h"

#include <stdlib.h>

#include "resources.h"
#include "pac_man/pac_man.h"
#include "pac_man/pac_level.h"

#define GAME_OPTION_COUNT (2)
#define GAME_OPTION_PAC_MAN (0)
#define GAME_OPTION_FROGGER (1)

typedef struct GameOption GameOption;

struct GameOption {
  const char *title;
  CGL_Animation *leftAnim;
  CGL_Animation *rightAnim;
  CGL_Screen *screen;
};

typedef struct MenuScreenData MenuScreenData;

struct MenuScreenData {
  CGL_Font *font;
  SDL_Color titleClr;
  int titleTime;
  GameOption opts[GAME_OPTION_COUNT];
  int optIdx;
};

int MenuScreenInit(CGL_Screen *screen, CGL_Context *ctx)
{
  SDL_Renderer *rend = CGL_ScreenGetData(screen);
  if(rend == NULL)
    return -1;

  MenuScreenData *data = (MenuScreenData*)malloc(sizeof(MenuScreenData));
  if(data == NULL)
    return -1;

  CGL_Animation *leftGhostAnim = NULL;
  CGL_Animation *rightGhostAnim = NULL;
  CGL_SpriteSheet *ghostSheet = ResourcesGetSpriteSheet(SPRITE_SHEET_PAC_MAN_GHOSTS);
  
  leftGhostAnim = CGL_InitAnimation(2, 10, true);
  CGL_SpriteSheetGetSpriteAt(ghostSheet, 0, 0, CGL_AnimationGetFrame(leftGhostAnim, 0));
  CGL_SpriteSheetGetSpriteAt(ghostSheet, 1, 0, CGL_AnimationGetFrame(leftGhostAnim, 1));

  rightGhostAnim = CGL_InitAnimation(2, 10, true);
  CGL_SpriteSheetGetSpriteAt(ghostSheet, 2, 1, CGL_AnimationGetFrame(rightGhostAnim, 0));
  CGL_SpriteSheetGetSpriteAt(ghostSheet, 3, 1, CGL_AnimationGetFrame(rightGhostAnim, 1));

  CGL_Screen *pacManScreen = CGL_CreateScreen(PacManScreenInit,
                                              PacManScreenUpdate,
                                              PacManScreenRender,
                                              PacManScreenDestroy,
                                              rend);
  if(pacManScreen != NULL)
  {
    CGL_ScreenInit(pacManScreen, ctx);
    //FIX ME: Move this to level select screen eventually
    PacManLevel *lvl = LoadPacManLevel("res/json/pac_man/level0.json");
    if(lvl != NULL)
    {
      CGL_LogInfo("Successfully parsed level json.");
      PacManScreenSetLevel(pacManScreen, lvl, ctx);
    }
    else
      CGL_LogError("Failed to parse level json!");
  }
  else
    CGL_LogError("ERROR: Failed to create Pac-Man screen!");

  CGL_Animation *leftFrogAnim = NULL;
  CGL_Animation *rightFrogAnim = NULL;
  CGL_SpriteSheet *frogSheet = ResourcesGetSpriteSheet(SPRITE_SHEET_FROGGER_FROG);

  leftFrogAnim = CGL_InitAnimation(2, 10, true);
  CGL_SpriteSheetGetSpriteAt(frogSheet, 2, 1, CGL_AnimationGetFrame(leftFrogAnim, 0));
  CGL_SpriteSheetGetSpriteAt(frogSheet, 3, 1, CGL_AnimationGetFrame(leftFrogAnim, 1));

  rightFrogAnim = CGL_InitAnimation(2, 10, true);
  CGL_SpriteSheetGetSpriteAt(frogSheet, 2, 0, CGL_AnimationGetFrame(rightFrogAnim, 0));
  CGL_SpriteSheetGetSpriteAt(frogSheet, 3, 0, CGL_AnimationGetFrame(rightFrogAnim, 1));

  data->font = ResourcesGetFont(FONT_PAC_MAN);
  data->titleClr = (SDL_Color){
    .r = 255,
    .g = 0,
    .b = 0,
    .a = 255
  };
  data->titleTime = 0;

  data->optIdx = 0;
  data->opts[GAME_OPTION_PAC_MAN] = (GameOption){
    .title = "PAC-MAN",
    .leftAnim = leftGhostAnim,
    .rightAnim = rightGhostAnim,
    .screen = pacManScreen
  };
  data->opts[GAME_OPTION_FROGGER] = (GameOption){
    .title = "FROGGER",
    .leftAnim = leftFrogAnim,
    .rightAnim = rightFrogAnim,
    .screen = NULL
  };

  CGL_ScreenSetData(screen, data);
  return 0;
}

void MenuScreenUpdate(CGL_Screen *screen, CGL_Context *ctx)
{
  MenuScreenData *data = CGL_ScreenGetData(screen);

  data->titleTime++;
  if(data->titleTime == 15)
  {
    data->titleTime = 0;
    data->titleClr = (SDL_Color){
      .r = rand(),
      .g = rand(),
      .b = rand(),
      .a = 255,
    };
  }

  GameOption opt = data->opts[data->optIdx];

  if(CGL_ContextInputJustSet(ctx, CGL_INPUT_P1_START))
  {
    if(opt.screen != NULL)
      CGL_ContextSetScreen(ctx, opt.screen);
    else
      CGL_LogError("ERROR: Can't start game, because its screen is NULL!");
  }
  else if(CGL_ContextInputJustSet(ctx, CGL_INPUT_DOWN))
  {
    data->optIdx++;
    if(data->optIdx >= GAME_OPTION_COUNT)
      data->optIdx = 0;
  }
  else if(CGL_ContextInputJustSet(ctx, CGL_INPUT_UP))
  {
    data->optIdx--;
    if(data->optIdx < 0)
      data->optIdx = GAME_OPTION_COUNT-1;
  }

  if(opt.leftAnim != NULL)
    CGL_AnimationUpdate(opt.leftAnim);
  if(opt.rightAnim != NULL)
    CGL_AnimationUpdate(opt.rightAnim);
}

void MenuScreenRender(CGL_Screen *screen, CGL_Context *ctx)
{
  MenuScreenData *data = CGL_ScreenGetData(screen);
  
  CGL_DrawString(ctx, data->font, "CROW ARCADE!", 4, 4, 18, 18, &(data->titleClr));

  int startY = GAME_HEIGHT/2 - GAME_OPTION_COUNT*16/2;
  for(int i = 0; i < GAME_OPTION_COUNT; i++)
  {
    GameOption opt = data->opts[i];
    const char *title = opt.title;
    int len = 12 * strlen(title);
    int x = GAME_WIDTH/2-len/2;
    int y = startY+i*16;
    CGL_DrawString(ctx, data->font, title, x, y, 12, 12, NULL);

    if(i == data->optIdx)
    {
      if(opt.leftAnim != NULL)
        CGL_DrawAnimation(ctx, opt.leftAnim, x-16, y-2, 16, 16);
      if(opt.rightAnim != NULL)
        CGL_DrawAnimation(ctx, opt.rightAnim, x+len, y-2, 16, 16);
    }
  }
}

void MenuScreenDestroy(CGL_Screen *screen)
{
  MenuScreenData *data = CGL_ScreenGetData(screen);
  if(data == NULL)
    return;

  for(int i = 0; i < GAME_OPTION_COUNT; i++)
  {
    GameOption opt = data->opts[i];
    if(opt.screen != NULL)
      CGL_DestroyScreen(opt.screen);

    if(opt.leftAnim != NULL)
      CGL_DestroyAnimation(opt.leftAnim);
    if(opt.rightAnim != NULL && opt.rightAnim != opt.leftAnim)
      CGL_DestroyAnimation(opt.rightAnim);
  }

  free(data);
}
