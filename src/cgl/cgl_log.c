
#include "cgl_log.h"

#include <stdio.h>
#include <string.h>
#include <time.h>

const char* CGL_LogGetLevelLabel(CGL_LogLevel lvl)
{
  switch(lvl)
  {
    case CGL_LOG_INFO:    return "[INFO]   ";
    case CGL_LOG_WARNING: return "[WARNING]";
    case CGL_LOG_ERROR:   return "[ERROR]  ";
    default:              return "[???]    ";
  }
}

void CGL_Log(CGL_LogLevel lvl, const char *fmt, ...)
{
  time_t now = time(NULL);
  struct tm *tm = localtime(&now);
  char timeBuff[64];

  strftime(timeBuff, sizeof(timeBuff), "%H:%M:%S", tm);

  printf("%s [%s]: ", CGL_LogGetLevelLabel(lvl), timeBuff);

  va_list args;
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);

  printf("\n");
}

void CGL_LogInfo(const char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  CGL_Log(CGL_LOG_INFO, fmt, args);
  va_end(args);
}

void CGL_LogWarning(const char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  CGL_Log(CGL_LOG_WARNING, fmt, args);
  va_end(args);
}

void CGL_LogError(const char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  CGL_Log(CGL_LOG_ERROR, fmt, args);
  va_end(args);
}

