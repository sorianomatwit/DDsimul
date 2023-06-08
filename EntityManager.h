#ifndef ENTITY_MANAGER
#define ENTITY_MANAGER

#include "ComponentManager.h"

struct Entity
{
public:
	unsigned short _id;
	Entity(unsigned short newId);

	template <typename T>
	bool HasComponent();// { return ComponentManager::GetComponentSet<T>() }//.HasEntity(this);
	template <typename T>
	void AddComponent();// { ComponentManager.GetComponentSet<T>().AddEntity(this, newComponent) };
	template <typename T>
	void RemoveComponent();// { ComponentManager::GetComponentSet<T>().RemoveEntity(this) };
	template <typename T>
	T* GetComponent(); //{ ComponentManager.GetComponentSet<T>().GetComponent(this) };
	void Destroy();
	bool Equals(struct Entity* other);
};


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
	BitTracker(unsigned short bits) {
		this->bits = bits;
	}
	BitTracker() { this->bits = 0; }
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
	static void DestroyEntity(Entity* entity);
};

template <typename T>
bool Entity::HasComponent() {
	ComponentSet<T>* componentSet = ComponentManager::GetComponentSet<T>();
	return componentSet->HasEntity(this->_id);
}

template <typename T>
void Entity::AddComponent() {
	ComponentSet<T>* componentSet = ComponentManager::GetComponentSet<T>();
	componentSet->AddEntity(this->_id);
	EntityManager::AddComponentKey(this, componentSet->key);

}

template <typename T>
void Entity::RemoveComponent() {
	ComponentSet<T>* componentSet = ComponentManager::GetComponentSet<T>();
	componentSet->RemoveEntity(this->_id);
	EntityManager::RemoveComponentKey(this, componentSet->key);
}

template <typename T>
T* Entity::GetComponent() {
	ComponentSet<T>* componentSet = ComponentManager::GetComponentSet<T>();
	return componentSet->GetComponent(this->_id);
}


//BitTracker EntityManager::activeEntityBits[BitTracker::TOTAL_BITS];
//PackedArray<unsigned short> EntityManager::deadEntities;
//unsigned short EntityManager::entityCount = 0;
#endif // !ENTITY_MANAGER