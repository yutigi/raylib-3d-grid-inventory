#pragma once

#include <cmath>

#include "raylib.h"

Vector3 SnapGrid(const Vector3 Pos,const float OffsetUp, const float GirdSize)
{
    return Vector3{
        round(Pos.x / GirdSize) * GirdSize,
        (round(Pos.y / GirdSize) * GirdSize) + OffsetUp,
        round(Pos.z / GirdSize) * GirdSize
    };
}