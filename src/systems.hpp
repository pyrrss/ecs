#pragma once

#include "ecs.hpp"
#include "componentPool.hpp"
#include "components.hpp"
#include "types.hpp"

// TODO: los sistemas no deberian recibir explicitamente los pools de componentes que necesitan,
// sino un manejador general que almacene los pools y dentro de ellos pueda acceder a sus componentes
// mañana lo cambio

// TODO: hacer namespaces para cada system y listo. por ahora como son stateless prefiero asi

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



