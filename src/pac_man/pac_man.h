
#ifndef PAC_MAN_H
#define PAC_MAN_H

#include "../cgl/cgl.h"
#include "pac_level.h"

void PacManScreenSetLevel(CGL_Screen *screen, PacManLevel *lvl);

int PacManScreenInit(CGL_Screen *screen);
void PacManScreenUpdate(CGL_Screen *screen, CGL_Context *ctx);
void PacManScreenRender(CGL_Screen *screen, CGL_Context *ctx);
void PacManScreenDestroy(CGL_Screen *screen);

#endif
