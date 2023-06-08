
#include "EntityManager.h"

 BitTracker EntityManager::activeEntityBits[Sets::MAX_ENTITIES];
 PackedArray<unsigned short> EntityManager::deadEntities;
 unsigned short EntityManager::entityCount = 0;

Entity EntityManager::CreateEntity()
{
    unsigned short id = EntityManager::entityCount;
    EntityManager::entityCount++;
    return Entity(id);
}
void EntityManager::AddComponentKey(Entity *entity, int componentKey) { EntityManager::activeEntityBits[entity->_id].Add(componentKey); };
void EntityManager::RemoveComponentKey(Entity *entity, int componentKey) { EntityManager::activeEntityBits[entity->_id].Remove(componentKey); };
void EntityManager::DestroyEntity(Entity *entity)
{
    EntityManager::deadEntities.Add(entity->_id);
    EntityManager::entityCount--;
    BitTracker *bits = &EntityManager::activeEntityBits[entity->_id];
    for (int i = 0; i < BitTracker::TOTAL_BITS; i++)
    {
        if (bits->Has(i))
        {
            std::function<bool(unsigned short)> ptr = ComponentManager::allEntityRemoves[i];
            (ptr)(entity->_id);
        }
    }
}

Entity::Entity(unsigned short id) {
    this->_id = id;
}

bool Entity::Equals(Entity* other) {
    return this->_id == other->_id;
}

void Entity::Destroy() {
    EntityManager::DestroyEntity(this);
}