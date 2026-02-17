#pragma once

#include "../include/ecs.hpp"

namespace MovementSystem
{
    void move(ECS &ecs, float delta_time);

}

namespace RenderSystem
{
    void render(ECS &ecs);
}

namespace LifeTimeSystem
{
    void update(ECS &ecs, float delta_time);
}

namespace BoundsCollisionSystem
{
    void handle_collisions(ECS &ecs);
}
