
#ifndef CGL_LOGGER_H
#define CGL_LOGGER_H

#include <stdarg.h>

typedef enum {
  CGL_LOG_INFO,
  CGL_LOG_WARNING,
  CGL_LOG_ERROR
} CGL_LogLevel;

void CGL_Log(CGL_LogLevel lvl, const char *fmt, ...);
void CGL_LogInfo(const char *fmt, ...);
void CGL_LogWarning(const char *fmt, ...);
void CGL_LogError(const char *fmt, ...);

#endif
