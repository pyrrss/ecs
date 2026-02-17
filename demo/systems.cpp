#include <raylib.h>

#include "systems.hpp"
#include "componentPool.hpp"
#include "components.hpp"
#include "types.hpp"

// NOTE: cada sistema itera sobre el conjunto denso de uno de los componentes que necesite (el más pequeño)
// y así se asegura minimizar número de iteraciones

void MovementSystem::move(ECS &ecs, float delta_time)
{
    auto& transform_dense_vector = ecs.get_component_dense_vector<TransformComponent>();
    auto& physics_dense_vector = ecs.get_component_dense_vector<PhysicsComponent>();
    const float GRAVITY = 512.0f;

    if (transform_dense_vector.size() < physics_dense_vector.size())
    {
        for (auto& transform_slot : transform_dense_vector)
        {
            EntityId entity_id = transform_slot.entity_id;
            if (!ecs.has_component<PhysicsComponent>(entity_id))
            {
                continue; // -> si la entidad no tiene physics component, saltar
            }

            TransformComponent& transform = transform_slot.component;
            PhysicsComponent& physics = ecs.get_component<PhysicsComponent>(entity_id);
            
            // se aplica vel.
            transform.x += physics.velocity_x * delta_time;
            transform.y += physics.velocity_y * delta_time;
            
            // se aplica gravedad
            physics.velocity_y += GRAVITY * delta_time;
            
        }
    }
    else
    {
        for (auto& physics_slot : physics_dense_vector)
        {
            EntityId entity_id = physics_slot.entity_id;
            if (!ecs.has_component<TransformComponent>(entity_id)) continue; // -> saltar si no tiene transform comp.

            PhysicsComponent& physics = physics_slot.component;
            TransformComponent& transform = ecs.get_component<TransformComponent>(entity_id);

            // se aplica vel.
            transform.x += physics.velocity_x * delta_time;
            transform.y += physics.velocity_y * delta_time;
            
            // se aplica gravedad
            physics.velocity_y += GRAVITY * delta_time;
        }
    }

}

void RenderSystem::render(ECS &ecs)
{
    auto& transform_dense_vector = ecs.get_component_dense_vector<TransformComponent>();
    auto& texture_dense_vector = ecs.get_component_dense_vector<TextureComponent>();
    
    if (transform_dense_vector.size() < texture_dense_vector.size())
    {
        for (auto& transform_slot : transform_dense_vector)
        {
            EntityId entity_id = transform_slot.entity_id;
            if (!ecs.has_component<TextureComponent>(entity_id)) continue; // -> se salta si no tiene texture comp.
            
            TransformComponent& transform = transform_slot.component;
            TextureComponent& texture = ecs.get_component<TextureComponent>(entity_id);
            
            Color color = { texture.color.r, texture.color.g, texture.color.b, (unsigned char) texture.alpha };
            DrawCircleV({ transform.x, transform.y }, texture.width / 2, color);
        }
    }
    else
    {
        for (auto& texture_slot : texture_dense_vector)
        {
            EntityId entity_id = texture_slot.entity_id;
            if (!ecs.has_component<TransformComponent>(entity_id)) continue;

            TextureComponent& texture = texture_slot.component;
            TransformComponent& transform = ecs.get_component<TransformComponent>(entity_id);
            Color color = { texture.color.r, texture.color.g, texture.color.b, (unsigned char) texture.alpha };
            
            DrawCircleV({ transform.x, transform.y }, texture.width / 2, color);
            
        }
    }

}

void LifeTimeSystem::update(ECS &ecs, float delta_time)
{
    auto& life_time_dense_vector = ecs.get_component_dense_vector<LifeTimeComponent>();

    std::vector<EntityId> entities_to_destroy;
    for (auto& life_time_slot : life_time_dense_vector)
    {
        EntityId entity_id = life_time_slot.entity_id;
        if (!ecs.has_component<TextureComponent>(entity_id)) continue;

        LifeTimeComponent& life_time = life_time_slot.component;
        TextureComponent& texture = ecs.get_component<TextureComponent>(entity_id);

        // se reduce tiempo de vida restante de la entidad
        life_time.remaining -= delta_time;
        float life_ratio = life_time.remaining / life_time.max;
        
        texture.alpha = life_ratio * 255.0f; // -> fade out
        texture.width = life_ratio * 30.0f;

        if (life_time.remaining <= 0.0f)
        {
            entities_to_destroy.push_back(entity_id);
        }

    }

    // se destruyen entidades sin tiempo de vida restante
    for (EntityId entity_id : entities_to_destroy)
    {
        ecs.destroy_entity(entity_id);
    }

}

void BoundsCollisionSystem::handle_collisions(ECS &ecs)
{
    // idea es detectar colisiones con bordes de pantalla y hacer rebotes
    auto& transform_dense_vector = ecs.get_component_dense_vector<TransformComponent>();
    auto& physics_dense_vector = ecs.get_component_dense_vector<PhysicsComponent>();

    if (transform_dense_vector.size() < physics_dense_vector.size())
    {
        for (auto& transform_slot : transform_dense_vector)
        {
            EntityId entity_id = transform_slot.entity_id;
            if (!ecs.has_component<PhysicsComponent>(entity_id)) continue;
            
            TransformComponent& transform = transform_slot.component;
            PhysicsComponent& physics = ecs.get_component<PhysicsComponent>(entity_id);
            
            if (transform.y > GetScreenHeight())
            {
                transform.y = GetScreenHeight();
                physics.velocity_y *= -0.6f; // -> pierde 'energía' en cada rebote
            }

            if (transform.x < 0.0f)
            {
                transform.x = 0.0f;
                physics.velocity_x *= -0.6f;
            }

            if (transform.x > GetScreenWidth())
            {
                transform.x = GetScreenWidth();
                physics.velocity_x *= -0.6f;
            }

        }
    }
    else
    {
        for (auto& physics_slot : physics_dense_vector)
        {
            EntityId entity_id = physics_slot.entity_id;
            if (!ecs.has_component<TransformComponent>(entity_id)) continue;
            
            PhysicsComponent& physics = physics_slot.component;
            TransformComponent& transform = ecs.get_component<TransformComponent>(entity_id);
            
            if (transform.y > GetScreenHeight())
            {
                transform.y = GetScreenHeight();
                physics.velocity_y *= -0.6f; // -> pierde 'energía' en cada rebote
            }

            if (transform.x < 0.0f)
            {
                transform.x = 0.0f;
                physics.velocity_x *= -0.6f;
            }

            if (transform.x > GetScreenWidth())
            {
                transform.x = GetScreenWidth();
                physics.velocity_x *= -0.6f;
            }
        }
    }

}


