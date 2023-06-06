
#include "EntityManager.h"

 BitTracker EntityManager::activeEntityBits[BitTracker::TOTAL_BITS];
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
void EntityManager::Destroy(Entity *entity)
{
    EntityManager::deadEntities.Add(entity->_id);
    BitTracker *bits = &EntityManager::activeEntityBits[entity->_id];
    for (int i = 0; i < BitTracker::TOTAL_BITS; i++)
    {
        if (bits->Has(i))
        {
            RemoveEntity *ptr = ComponentManager::allEntityRemoves.Get(i);
            (*ptr)(entity->_id);
        }
    }
}