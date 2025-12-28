
#include "menu_screen.h"

typedef struct MenuScreenData MenuScreenData;

struct MenuScreenData {
  CGL_Font *font;
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
  CGL_ScreenSetData(screen, data);
  return 0;
}

void MenuScreenUpdate(CGL_Screen *screen, CGL_Context *ctx)
{

}

void MenuScreenRender(CGL_Screen *screen, CGL_Context *ctx)
{
  MenuScreenData *data = CGL_ScreenGetData(screen);
  //(CGL_Context *ctx, CGL_Font *font, const char *str, int x, int y, int charW, int charH, int r, int g, int b);
  CGL_DrawString(ctx, data->font, "HELLO WORLD!", 10, 10, 8, 8, 255, 0, 0);
}

void MenuScreenDestroy(CGL_Screen *screen)
{
  MenuScreenData *data = CGL_ScreenGetData(screen);
  if(data == NULL)
    return;

  CGL_DestroyFont(data->font);
  free(data);
}
