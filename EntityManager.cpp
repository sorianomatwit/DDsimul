
#include "EntityManager.h"
#include <algorithm>

std::array<BitTracker, Sets::MAX_ENTITIES> EntityManager::activeEntityBits;
PackedArray<unsigned short> EntityManager::deadEntities;
unsigned short EntityManager::entityCount = 0;


Entity * EntityManager::CreateEntity()
{
	unsigned short id = EntityManager::entityCount;
	EntityManager::activeEntityBits[id] = BitTracker();
	Entity * newEntity = new Entity(id);
	EntityManager::entityCount++;
	return newEntity;
}

void EntityManager::AddComponentKey(Entity* entity, uint8_t componentKey) {
	BitTracker * entityBits = &EntityManager::activeEntityBits[entity->_id];
	if (entityBits != nullptr && !entityBits->Has(componentKey)){
		entityBits->Add(componentKey);
	}
};

void EntityManager::RemoveComponentKey(Entity* entity, uint8_t componentKey) { 
	BitTracker* entityBits = &EntityManager::activeEntityBits[entity->_id];
	if (entityBits != nullptr && entityBits->Has(componentKey)) {
		entityBits->Remove(componentKey);
	}
};

void EntityManager::DestroyEntity(Entity* entity)
{

	EntityManager::deadEntities.Add(entity->_id);
	BitTracker* bits = &EntityManager::activeEntityBits[entity->_id];
	for (uint8_t i = 0; i < BitTracker::TOTAL_BITS; i++)
	{
		if (bits->Has(i))
		{
			bits->Remove(i);
			auto ptr = ComponentManager::allEntityRemoves[i];
			ptr(entity->_id);
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