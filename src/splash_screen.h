
#ifndef SPLASH_SCREEN_H
#define SPLASH_SCREEN_H

#include "cgl/cgl.h"

int SplashScreenInit(CGL_Screen *screen);
void SplashScreenUpdate(CGL_Screen *screen);
void SplashScreenRender(CGL_Screen *screen);
void SplashScreenDestroy(CGL_Screen *screen);

#endif
