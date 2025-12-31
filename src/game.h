
#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

#include "cJSON.h"

#define VECTOR_2I_JSON_KEY_X ("x")
#define VECTOR_2I_JSON_KEY_Y ("y")

typedef struct Vector2I Vector2I;

struct Vector2I {
  int x;
  int y;
};

bool ParseVector2I(cJSON *json, Vector2I *v);

int DistanceSquaredVector2I(const Vector2I *v1, const Vector2I *v2);
bool IsEqualVector2I(const Vector2I *v1, const Vector2I *v2);
void AddVector2I(const Vector2I *v1, const Vector2I *v2, Vector2I *res);
void SubtractVector2I(const Vector2I *v1, const Vector2I *v2, Vector2I *res);
void ScaleVector2I(const Vector2I *v, int scale, Vector2I *res);
void AbsVector2I(const Vector2I *v, Vector2I *res);
int MagnitudeSquaredVector2I(const Vector2I *v);

#endif
