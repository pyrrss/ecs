#include <raylib.h>

#include "components.hpp"
#include "systems.hpp"

void spawn_particles(ECS &ecs, float spawn_rate, float delta_time)
{
    static float spawn_timer = 0.0f;
    spawn_timer += delta_time;
    
    if (spawn_timer < spawn_rate) return;
    spawn_timer = 0.0f;
    
    // se crea nueva entidad y se inicializan componentes
    EntityId new_entity = ecs.create_entity();
    Vector2 spawn_position = {
        (float) GetRandomValue(0, GetScreenWidth()),
        -10.0f
    };

    TransformComponent transform_component = {
        spawn_position.x,
        spawn_position.y,
        0.0f,
        1.0f,
        1.0f
    };
    
    PhysicsComponent physics_component = {
        (float) GetRandomValue(-100, 100),
        (float) GetRandomValue(100, 250),
        1.0f
    };

    TextureComponent texture_component = {
        (Color){
            (unsigned char) GetRandomValue(0, 255),
            (unsigned char) GetRandomValue(0, 255),
            (unsigned char) GetRandomValue(0, 255),
            255
        },
        30.0f,
        30.0f,
        255
    };

    LifeTimeComponent life_time_component = {
        10.0f,
        10.0f
    };
 
    // se añaden los componentes a la entidad
    ecs.add_component<TransformComponent>(new_entity) = transform_component;
    ecs.add_component<PhysicsComponent>(new_entity) = physics_component;
    ecs.add_component<TextureComponent>(new_entity) = texture_component;
    ecs.add_component<LifeTimeComponent>(new_entity) = life_time_component;
};

int main()
{
    InitWindow(1280, 720, "ECS Demo");
    SetRandomSeed(time(NULL));
    SetTargetFPS(60);

    // -- setup ECS --
    ECS ecs = ECS();
    ecs.register_component<TransformComponent>();
    ecs.register_component<PhysicsComponent>();
    ecs.register_component<TextureComponent>();
    ecs.register_component<LifeTimeComponent>();

    // -- game loop --
    while (!WindowShouldClose())
    {
        // update
        float delta_time = GetFrameTime();
        spawn_particles(ecs, 0.0f, delta_time);
        MovementSystem::move(ecs, delta_time);
        LifeTimeSystem::update(ecs, delta_time);
        BoundsCollisionSystem::handle_collisions(ecs);

        // render
        BeginDrawing();
        {
            ClearBackground(BLACK);
            
            const char* fps_message = TextFormat("FPS: %d", GetFPS());
            DrawText(fps_message, 10, 10, 20, RAYWHITE);

            RenderSystem::render(ecs);
            
        }
        EndDrawing();
    }

    return 0;
}
