
#ifndef MENU_SCREEN_H
#define MENU_SCREEN_H

#include "cgl.h"

int MenuScreenInit(CGL_Screen *screen);
void MenuScreenUpdate(CGL_Screen *screen);
void MenuScreenRender(CGL_Screen *screen);
void MenuScreenDestroy(CGL_Screen *screen);

#endif
