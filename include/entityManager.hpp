#pragma once

#include <assert.h>
#include <vector>

#include "types.hpp"

using namespace ecs_types;

class EntityManager
{
    private:
        std::vector<EntityId> m_available_entities; // -> entidades disponibles (se usa como stack)
        std::vector<Signature> m_signatures; // -> firma de cada entidad (qué componentes tiene),
                                             // cada bit representa si tiene un componente o no,
                                             // indexado por type id del componente
        
        std::vector<bool> m_alive_entities; // -> vector de flags para saber si entidades vivas o no 

        size_t m_living_entity_count = 0; // -> número de entidades vivas
    
    public:
        EntityManager();
        ~EntityManager();

        EntityId create_entity();
        void destroy_entity(EntityId entity_id);
        bool is_entity_alive(EntityId entity_id) const;


        void set_signature(EntityId entity_id, Signature signature);
        Signature get_signature(EntityId entity_id) const;
        void add_component_to_signature(EntityId entity_id, ComponentTypeId type_id);
        void remove_component_from_signature(EntityId entity_id, ComponentTypeId type_id);

        uint32_t get_living_entity_count() const;

};



