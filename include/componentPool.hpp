#pragma once

#include <vector>
#include <cassert>

#include "types.hpp"

using namespace ecs_types;

class IComponentPool
{
    public:
        virtual ~IComponentPool() = default;
        virtual void remove_component(EntityId entity_id) = 0;
};

template <typename Component>
struct DenseSlot
{
    EntityId entity_id; // -> id de la entidad a la que pertenece el componente
    Component component; // -> componente en sí
};

template <typename Component>
class ComponentPool : public IComponentPool
{
    private:
        // NOTE: oportunidad de optimización: paginar m_sparse (supongo que afectará cuando hayan muchas entidades)
    
        std::vector<uint32_t> m_sparse; // -> sparse vector := cada índice es un EntityId, el valor es un índice del vector denso
        std::vector<DenseSlot<Component>> m_dense; // -> dense vector := cada slot tiene un componente 
                                                   // y el id de la entidad a la que pertenece
    
    public:
        ComponentPool()
        {
            // se inicializa sparse vector con valores inválidos
            m_sparse.assign(MAX_ENTITIES, INVALID);
        }
        ~ComponentPool() {};
        
        void add_component(EntityId entity_id)
        {
            assert(entity_id < MAX_ENTITIES && "Entidad inválida");
            assert(m_sparse[entity_id] == INVALID && "Entidad ya tiene este componente");
            assert(m_dense.size() < MAX_ENTITIES && "Límite de componentes alcanzado");

            // se crea un nuevo slot para agregar al vector denso
            DenseSlot<Component> new_slot = {entity_id, Component()}; // se inicializa component con default constructor
            m_dense.push_back(new_slot);

            // se agrega el índice del slot en el vector denso al sparse, indexado por id de entidad
            m_sparse[entity_id] = m_dense.size() - 1;
        }

        void remove_component(EntityId entity_id) override
        {
            assert(entity_id < MAX_ENTITIES && "Entidad inválida");

            uint32_t dense_index = m_sparse[entity_id];
            if (dense_index == INVALID) return; // lo hago con if solo para no romper programa con assert en caso que haya alguna inconsistencia,
                                                // pero no deberia pasar ya que previamente se deberia haber revisado la signature de la entidad
                                                // y solo se removerian componentes que sí tiene 
            uint32_t last_dense_index = m_dense.size() - 1;
            
            // se reemplaza el slot a eliminar por el último slot del vector denso
            m_dense[dense_index] = m_dense[last_dense_index];
            
            // se actualiza el sparse vector para que en la posición del entity id del slot
            // que se movió, apunte a su nuevo índice en el vector denso
            EntityId last_entity_id = m_dense[dense_index].entity_id;
            m_sparse[last_entity_id] = dense_index;
            
            // se elimina último slot del vector denso
            m_dense.pop_back();
        
            // se marca índice del componente eliminado como inválido en el sparse vector
            m_sparse[entity_id] = INVALID;
        }
        
        Component& get_component(EntityId entity_id)
        {
            assert(entity_id < MAX_ENTITIES && "Entidad inválida");

            uint32_t dense_index = m_sparse[entity_id];
            assert(dense_index != INVALID && "Entidad no tiene este componente");

            return m_dense[dense_index].component;
        }

        bool has_component(EntityId entity_id) const
        {
            assert(entity_id < MAX_ENTITIES && "Entidad inválida");

            uint32_t dense_index = m_sparse[entity_id];
            if (dense_index == INVALID) return false;

            // se revisa si el id de la entidad en el slot del vector denso
            // calza con el id de la entidad de la que se está consultando
            // (esto no debería fallar)
            assert(m_dense[dense_index].entity_id == entity_id && "Inconsistencia de componentes en sparse y dense vector");
            return true;
        }

        std::vector<DenseSlot<Component>>& get_dense_vector()
        {
            return m_dense;
        }

};
