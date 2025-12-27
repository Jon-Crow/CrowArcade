
#ifndef MENU_SCREEN_H
#define MENU_SCREEN_H

#include "cgl/cgl.h"

int MenuScreenInit(CGL_Screen *screen);
void MenuScreenUpdate(CGL_Screen *screen, CGL_Context *ctx);
void MenuScreenRender(CGL_Screen *screen, CGL_Context *ctx);
void MenuScreenDestroy(CGL_Screen *screen);

#endif
