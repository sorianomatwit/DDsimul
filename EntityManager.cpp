
#include "EntityManager.h"
#include <algorithm>

std::array<BitTracker, Sets::MAX_ENTITIES> EntityManager::activeEntityBits;
/*std::fill(EntityManager::activeEntityBits, EntityManager::activeEntityBits + Sets::MAX_ENTITIES, BitTracker());*/
PackedArray<unsigned short> EntityManager::deadEntities;
unsigned short EntityManager::entityCount = 0;

Entity EntityManager::CreateEntity()
{
	unsigned short id = EntityManager::entityCount;
	EntityManager::activeEntityBits[id] = BitTracker();
	Entity newEntity = Entity(id);
	EntityManager::entityCount++;
	return newEntity;
}
void EntityManager::AddComponentKey(Entity* entity, int componentKey) {
	BitTracker * entityBits = &EntityManager::activeEntityBits[entity->_id];
	if (entityBits != nullptr && !entityBits->Has(componentKey)){
		entityBits->Add(componentKey);
	}
};
void EntityManager::RemoveComponentKey(Entity* entity, int componentKey) { 
	BitTracker* entityBits = &EntityManager::activeEntityBits[entity->_id];
	if (entityBits != nullptr && entityBits->Has(componentKey)) {
		entityBits->Remove(componentKey);
	}
};
void EntityManager::DestroyEntity(Entity* entity)
{
	EntityManager::deadEntities.Add(entity->_id);
	BitTracker* bits = &EntityManager::activeEntityBits[entity->_id];
	for (int i = 0; i < BitTracker::TOTAL_BITS; i++)
	{
		if (bits->Has(i))
		{
			std::function<bool(unsigned short)> ptr = ComponentManager::allEntityRemoves[i];
			(ptr)(entity->_id);
		}
	}
	EntityManager::activeEntityBits[entity->_id] = BitTracker();
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