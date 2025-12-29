
#ifndef PAC_MAN_H
#define PAC_MAN_H

#include "../cgl/cgl.h"

int PacManScreenInit(CGL_Screen *screen);
void PacManScreenUpdate(CGL_Screen *screen, CGL_Context *ctx);
void PacManScreenRender(CGL_Screen *screen, CGL_Context *ctx);
void PacManScreenDestroy(CGL_Screen *screen);

#endif
