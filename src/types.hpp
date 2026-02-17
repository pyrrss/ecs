#pragma once

#include <bitset>
#include <cstdint>


namespace ecs_types
{
    using EntityId = std::uint32_t;
    using ComponentTypeId = std::uint8_t;

    const EntityId MAX_ENTITIES = 5000; // -> número máx. de entidades que se pueden crear
    const ComponentTypeId MAX_COMPONENTS = 32; // -> número máx. de tipos de componentes por entidad
    const uint32_t INVALID = UINT32_MAX; // -> valor inválido para índices

    using Signature = std::bitset<MAX_COMPONENTS>; // -> signature es una cadena de bits asociada a cada entidad que indica
                                                   // qué componentes tiene (cada bit representa a un tipo de componente)

    

}
