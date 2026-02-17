#include "../include/entityManager.hpp"
#include "../include/types.hpp"
#include <sys/types.h>


EntityManager::EntityManager()
{
    for (EntityId entity = 0; entity < MAX_ENTITIES; entity++)
    {   
        // se inicializan todos los ids de entidades como disponibles
        m_available_entities.push_back(entity);
    }
    
    m_signatures.resize(MAX_ENTITIES);
    m_alive_entities.resize(MAX_ENTITIES, false);
}

EntityId EntityManager::create_entity()
{
    // NOTE: se trata al vector como un stack
    assert(m_living_entity_count < MAX_ENTITIES && "Límite de entidades alcanzado");
    assert(!m_available_entities.empty() && "No hay entidades disponibles");

    EntityId entity_id = m_available_entities.back();
    m_available_entities.pop_back(); // -> se saca la entidad del stack de entidades disponibles
    m_living_entity_count++;
    
    m_signatures[entity_id].reset();
    m_alive_entities[entity_id] = true; // -> se marca la entidad como viva

    return entity_id;
}

void EntityManager::destroy_entity(EntityId entity_id)
{
    assert(entity_id < MAX_ENTITIES && "Entidad inválida");
    assert(m_alive_entities[entity_id] && "Entidad ya destruida previamente");

    m_signatures[entity_id].reset(); // -> se resetea firma de componentes de la entidad
    m_alive_entities[entity_id] = false; // -> se marca entidad como muerta
    m_available_entities.push_back(entity_id); // -> se devuelve la entidad al stack de disponibles
    m_living_entity_count--;
}

bool EntityManager::is_entity_alive(EntityId entity_id) const
{
    assert(entity_id < MAX_ENTITIES && "Entidad inválida");
    return m_alive_entities[entity_id];
}

void EntityManager::set_signature(EntityId entity_id, Signature signature)
{
    assert(entity_id < MAX_ENTITIES && "Entidad inválida");
    
    m_signatures[entity_id] = signature; // -> se asigna la firma de componentes a la entidad
}

Signature EntityManager::get_signature(EntityId entity_id) const
{
    assert(entity_id < MAX_ENTITIES && "Entidad inválidad");
    return m_signatures[entity_id]; // -> se retorna la firma de componentes de la entidad
}

void EntityManager::add_component_to_signature(EntityId entity_id, ComponentTypeId type_id)
{
    assert(entity_id < MAX_ENTITIES && "Entidad inválida");
    assert(type_id < MAX_COMPONENTS && "Tipo de componente inválido");
    
    // se activa el bit correspondiente en la signature de la entidad
    m_signatures[entity_id].set(type_id, true);
}

void EntityManager::remove_component_from_signature(EntityId entity_id, ComponentTypeId type_id)
{
    assert(entity_id < MAX_ENTITIES && "Entidad inválida");
    assert(type_id < MAX_COMPONENTS && "Tipo de componente inválido");

    // se desactiva el bit correspondiente en la signature de la entidad
    m_signatures[entity_id].set(type_id, false);
}

uint32_t EntityManager::get_living_entity_count() const
{
    return m_living_entity_count; // -> se retorn num. entidades vivas
}


EntityManager::~EntityManager() {}
