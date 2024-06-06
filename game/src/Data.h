#pragma once

#include "raylib.h"

#define BOX_SIZE 2.f

typedef struct FBox
{
    Vector3 Position;
    float Scale;

    FBox(float x,float y,float z)
    {
        Position = Vector3{x,y,z};
        Scale = BOX_SIZE;
    }
    ~FBox()
    {
        
    }
    Vector3 GetPosition() const
    {
        return Position;
    }
    Vector3 GetScale() const
    {
        return Vector3{Scale,Scale,Scale};
    }
    float GetScalef() const
    {
        return Scale;
    }
    void SetPosition(const Vector3 NewPosition)
    {
        Position = NewPosition;
    }
    BoundingBox GetBoundingBox() const
    {
        return BoundingBox{
            Vector3{
                Position.x - Scale / 2, Position.y - Scale / 2,
                Position.z - Scale / 2
            },
            Vector3{
                Position.x + Scale / 2, Position.y + Scale / 2,
                Position.z + Scale / 2
            }
        };
    }
} FBox;
