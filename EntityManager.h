#ifndef ENTITY_MANAGER
#define ENTITY_MANAGER

#include "ComponentManager.h"
#include <iostream>
#include <array>
struct Entity
{
public:
	unsigned short _id;
	Entity(unsigned short newId);
	~Entity() = default;
	template <typename T>
	bool HasComponent();
	template <typename T>
	void AddComponent();
	template <typename T>
	void RemoveComponent();
	template <typename T>
	T* GetComponent(); 
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
	static std::array<BitTracker, Sets::MAX_ENTITIES> activeEntityBits;
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
	if (componentSet != nullptr) {
		componentSet->AddEntity(this->_id);
		std::cout << "Key" << ComponentManager::GetComponentKey<T>();
		EntityManager::AddComponentKey(this, ComponentManager::GetComponentKey<T>());
	}
	else {
		std::cerr << "Try to add component that does not exist in the ComponentManager";
	}

}

template <typename T>
void Entity::RemoveComponent() {
	ComponentSet<T>* componentSet = ComponentManager::GetComponentSet<T>();
	if (componentSet != nullptr) {
		componentSet->RemoveEntity(this->_id);
		EntityManager::RemoveComponentKey(this, componentSet->key);
	}
	else {
		std::cerr << "Try to add component that does not exist in the ComponentManager";
	}

}

template <typename T>
T* Entity::GetComponent() {
	ComponentSet<T>* componentSet = ComponentManager::GetComponentSet<T>();
	return componentSet->GetComponent(this->_id);
}

#endif // !ENTITY_MANAGER