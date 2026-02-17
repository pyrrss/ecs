#pragma once

#include <unordered_map>
#include <memory>
#include <cassert>

#include "componentPool.hpp"
#include "types.hpp"

using namespace ecs_types;

using ComponentPools = std::unordered_map<ComponentTypeId, std::unique_ptr<IComponentPool>>;

class ComponentManager
{
    private:
        ComponentPools m_component_pools; // -> mapea type ids a sus components pools
        
        ComponentTypeId m_next_component_type_id = 0; // -> id único para siguiente tipo de componente a registrar


    public:
        ComponentManager() = default;
        ~ComponentManager() {};
        
        template <typename Component>
        void register_component()
        {
            ComponentTypeId type_id = get_component_type_id<Component>();
            assert(m_component_pools.find(type_id) == m_component_pools.end() && "Componente ya registrado");
            assert(m_next_component_type_id < MAX_COMPONENTS && "Límite de tipos de componentes alcanzado");

            // se crea un nuevo component pool y se agrega al map
            std::unique_ptr<IComponentPool> new_pool = std::make_unique<ComponentPool<Component>>();
            m_component_pools.emplace(type_id, std::move(new_pool));
            
        }

        template <typename Component>
        ComponentTypeId get_component_type_id()
        {
            // static permite que type id se asigne una única vez por tipo de componente
            static ComponentTypeId type_id = m_next_component_type_id++; // -> se asigna y luego se incrementa (si es que no se ha asignado antes)
            return type_id;
        }

        template <typename Component>
        void add_component(EntityId entity_id)
        {
            assert(entity_id < MAX_ENTITIES && "Entidad no válida");

            ComponentTypeId type_id = get_component_type_id<Component>();
            assert(m_component_pools.find(type_id) != m_component_pools.end() && "Componente no registrado");
            
            IComponentPool *base_pool = m_component_pools[type_id].get();
            ComponentPool<Component> *pool = static_cast<ComponentPool<Component>*>(base_pool); // se castea al tipo específico de pool
            pool->add_component(entity_id);
        }

        template <typename Component>
        void remove_component(EntityId entity_id)
        {
            assert(entity_id < MAX_ENTITIES && "Entidad no válida");

            ComponentTypeId type_id = get_component_type_id<Component>();
            assert(m_component_pools.find(type_id) != m_component_pools.end() && "Componente no registrado");

            IComponentPool *base_pool = m_component_pools[type_id].get();
            ComponentPool<Component> *pool = static_cast<ComponentPool<Component>*>(base_pool);
            pool->remove_component(entity_id);
        }
        
        void remove_component_by_type_id(EntityId entity_id, ComponentTypeId type_id)
        {
            assert(entity_id < MAX_ENTITIES && "Entidad no válida");
            assert(type_id < MAX_COMPONENTS && "Tipo de componente no válido");
            assert(m_component_pools.find(type_id) != m_component_pools.end() && "Componente no registrado");
        
            IComponentPool *base_pool = m_component_pools[type_id].get();
            base_pool->remove_component(entity_id);
        }

        template <typename Component>
        Component& get_component(EntityId entity_id)
        {
            assert(entity_id < MAX_ENTITIES && "Entidad no válida");

            ComponentTypeId type_id = get_component_type_id<Component>();
            assert(m_component_pools.find(type_id) != m_component_pools.end() && "Componente no registrado");
            
            IComponentPool *base_pool = m_component_pools[type_id].get();
            ComponentPool<Component> *pool = static_cast<ComponentPool<Component>*>(base_pool);

            assert(pool->has_component(entity_id) && "Entidad no tiene este componente");
            return pool->get_component(entity_id);
        }

        template <typename Component>
        bool has_component(EntityId entity_id)
        {
            assert(entity_id < MAX_ENTITIES && "Entidad no válida");

            ComponentTypeId type_id = get_component_type_id<Component>();
            assert(m_component_pools.find(type_id) != m_component_pools.end() && "Componente no registrado");
        
            
            IComponentPool *base_pool = m_component_pools[type_id].get();
            ComponentPool<Component> *pool = static_cast<ComponentPool<Component>*>(base_pool);
        
            return pool->has_component(entity_id);
        }

        template <typename Component>
        std::vector<DenseSlot<Component>>& get_component_dense_vector()
        {
            ComponentTypeId type_id = get_component_type_id<Component>();
            assert(m_component_pools.find(type_id) != m_component_pools.end() && "Componente no registrado");

            IComponentPool *base_pool = m_component_pools[type_id].get();
            ComponentPool<Component> *pool = static_cast<ComponentPool<Component>*>(base_pool);
            return pool->get_dense_vector();
        }

};
