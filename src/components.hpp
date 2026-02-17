#pragma once

#include <raylib.h>

struct TransformComponent
{
    float x, y;
    float rotation;
    float scale_x, scale_y;
};

struct PhysicsComponent
{
    float velocity_x, velocity_y;
    float mass;
};

struct TextureComponent
{
    Color color;
    float width, height, alpha;
};

struct LifeTimeComponent
{
    float max;
    float remaining;
};
