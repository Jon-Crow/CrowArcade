
#include "menu_screen.h"

#include <stdlib.h>

typedef struct MenuScreenData MenuScreenData;

struct MenuScreenData {
  CGL_Font *font;
  SDL_Color titleClr;
  int titleTime;
};

int MenuScreenInit(CGL_Screen *screen)
{
  CGL_Texture *fontTx = CGL_LoadTexture(CGL_ScreenGetRenderer(screen), "../res/texture/pac_font.png");
  if(fontTx == NULL)
    return -1;

  CGL_SpriteSheet *fontSheet = CGL_CreateSpriteSheet(fontTx, 8, 8);
  if(fontSheet == NULL)
  {
    CGL_DestroyTexture(fontTx);
    return -1;
  }

  CGL_Font *font = CGL_CreateFont(fontSheet, "ABCDEFGHIJKLMNOPQRSTUVWXYZ!/-\"0123456789 ");
  if(font == NULL)
  {
    CGL_DestroySpriteSheet(fontSheet);
    CGL_DestroyTexture(fontTx);
    return -1;
  }

  MenuScreenData *data = (MenuScreenData*)malloc(sizeof(MenuScreenData));
  if(data == NULL)
  {
    CGL_DestroyFont(font);
    CGL_DestroySpriteSheet(fontSheet);
    CGL_DestroyTexture(fontTx);
    return -1;
  }

  data->font = font;
  data->titleClr = (SDL_Color){
    .r = 255,
    .g = 0,
    .b = 0,
    .a = 255
  };
  data->titleTime = 0;
  CGL_ScreenSetData(screen, data);
  return 0;
}

void MenuScreenUpdate(CGL_Screen *screen, CGL_Context *ctx)
{
  MenuScreenData *data = CGL_ScreenGetData(screen);

  data->titleTime++;
  if(data->titleTime == 30)
  {
    data->titleTime = 0;
    data->titleClr = (SDL_Color){
      .r = rand(),
      .g = rand(),
      .b = rand(),
      .a = 255,
    };
  }
}

void MenuScreenRender(CGL_Screen *screen, CGL_Context *ctx)
{
  MenuScreenData *data = CGL_ScreenGetData(screen);
  
  CGL_DrawString(ctx, data->font, "CROW ARCADE!", 4, 4, 18, 18, &(data->titleClr));
}

void MenuScreenDestroy(CGL_Screen *screen)
{
  MenuScreenData *data = CGL_ScreenGetData(screen);
  if(data == NULL)
    return;

  CGL_DestroyFont(data->font);
  free(data);
}
