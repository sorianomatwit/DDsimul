#ifndef ENTITY_MANAGER
#define ENTITY_MANAGER
#include "Entity.h"
#include "ComponentManager.h"

struct BitTracker
{
public:
	static const int TOTAL_BITS = 64;
	unsigned short bits;
	bool Has(int index) { return (bits & (1UL << index)) == (1UL << index); };
	void Add(int index) {
		if (!Has(index)) {
			bits = bits | (1UL << index);
		}
	}
	void Remove(int index) { if (Has(index)) bits = bits ^ (1UL << index); }
};

class EntityManager
{
public:
	static BitTracker activeEntityBits[];
	static PackedArray<unsigned short> deadEntities;
	static unsigned short entityCount;

	static Entity CreateEntity();
	static void AddComponentKey(Entity* entity, int componentKey);// = > activeEntityBits[entity.id].Add(componentKey);
	static void RemoveComponentKey(Entity* entity, int componentKey);// = > activeEntityBits[entity.id].Remove(componentKey);
	static void Destroy(Entity* entity);
};
//BitTracker EntityManager::activeEntityBits[BitTracker::TOTAL_BITS];
//PackedArray<unsigned short> EntityManager::deadEntities;
//unsigned short EntityManager::entityCount = 0;
#endif // !ENTITY_MANAGER