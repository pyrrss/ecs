#pragma once

#include <memory>

#include "entityManager.hpp"
#include "componentManager.hpp"
#include "types.hpp"
#include "componentPool.hpp"

using namespace ecs_types;

class ECS
{
    private:
        std::unique_ptr<EntityManager> m_entity_manager;
        std::unique_ptr<ComponentManager> m_component_manager;

    public:
        ECS()
        {
            m_entity_manager = std::make_unique<EntityManager>();
            m_component_manager = std::make_unique<ComponentManager>();
        }
        ~ECS() {};
        
        // -- entities --
        EntityId create_entity()
        {
            return m_entity_manager->create_entity();
        }

        void destroy_entity(EntityId entity_id)
        {
            assert(entity_id < MAX_ENTITIES && "Entidad no válida");
            assert(m_entity_manager->is_entity_alive(entity_id) && "Entidad ya destruida previametne");

            Signature entity_signature = m_entity_manager->get_signature(entity_id);
            
            // se recorre la signature (bitset) de la entidad y se eliminan los componentes cuyo bit esté en 1
            for (ComponentTypeId type_id = 0; type_id < MAX_COMPONENTS; type_id++)
            {
                if (entity_signature.test(type_id))
                {
                    // remover componente de entidad mediante type_id
                    m_component_manager->remove_component_by_type_id(entity_id, type_id);
                }
                
            }
            
            m_entity_manager->destroy_entity(entity_id);
        }

        // -- components --
        template <typename Component>
        void register_component()
        {
            m_component_manager->register_component<Component>();
        }
        
        template <typename Component>
        Component& add_component(EntityId entity_id)
        {
            m_component_manager->add_component<Component>(entity_id);
            
            // se actualiza signature de entidad
            ComponentTypeId type_id = m_component_manager->get_component_type_id<Component>();
            m_entity_manager->add_component_to_signature(entity_id, type_id);
        
            return m_component_manager->get_component<Component>(entity_id);
        }

        template <typename Component>
        void remove_component(EntityId entity_id)
        {
            m_component_manager->remove_component<Component>(entity_id);

            // se actualiza signature de entidad
            ComponentTypeId type_id = m_component_manager->get_component_type_id<Component>();
            m_entity_manager->remove_component_from_signature(entity_id, type_id);
        }
        
        template <typename Component>
        Component& get_component(EntityId entity_id)
        {
            assert(entity_id < MAX_ENTITIES && "Entidad no válida");
            return m_component_manager->get_component<Component>(entity_id);
        }
        
        template <typename Component>
        bool has_component(EntityId entity_id)
        {
            assert(entity_id < MAX_ENTITIES && "Entidad no válida");
            return m_component_manager->has_component<Component>(entity_id);
        }
        
        template <typename Component>
        std::vector<DenseSlot<Component>>& get_component_dense_vector()
        {
            return m_component_manager->get_component_dense_vector<Component>();
        }

};
