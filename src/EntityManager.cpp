#include "../include/ecs/EntityManager.hpp"

ee::ecs::EntityID ee::ecs::EntityManager::createEntity()
{
    EntityID id;
    if (!m_availableIDs.empty()){
        id = m_availableIDs.front();
        m_availableIDs.pop();
    }
    else
    id = m_nextID++;
    
    m_livingEntities.insert(id);
    return id;
}

void ee::ecs::EntityManager::DestroyEntity(EntityID _id)
{
    if (!isAlive(_id))
    return;
    m_availableIDs.push(_id);
    m_livingEntities.erase(_id);
}

ee::ecs::EntityID ee::ecs::EntityManager::reserveID()
{
    if (!m_availableIDs.empty()) {
        EntityID id = m_availableIDs.front();
        m_availableIDs.pop();
        return id;
    }
    return m_nextID++;
}

void ee::ecs::EntityManager::activateEntity(EntityID _id)
{
    m_livingEntities.insert(_id);
}

bool ee::ecs::EntityManager::isAlive(EntityID _id)
{
    return m_livingEntities.contains(_id);
}

size_t ee::ecs::EntityManager::getEntityCount() const
{
    return m_livingEntities.size();
}
