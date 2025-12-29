
#include "game.h"

bool ParseVector2I(cJSON *json, Vector2I *v)
{
  if(cJSON_IsObject(json))
  {
    cJSON *xJson = cJSON_GetObjectItemCaseSensitive(json, VECTOR_2I_JSON_KEY_X);
    if(cJSON_IsNumber(xJson))
      v->x = xJson->valueint;
    else
      return false;

    cJSON *yJson = cJSON_GetObjectItemCaseSensitive(json, VECTOR_2I_JSON_KEY_Y);
    if(cJSON_IsNumber(yJson))
      v->y = yJson->valueint;
    else
      return false;
  }
  else
    return false;

  return true;
}
