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
	T* AddComponent();
	template <typename T>
	void RemoveComponent();
	template <typename T>
	T* GetComponent();
	template <typename T>
	void SetComponent(T* data);
	void Destroy();
	bool Equals(struct Entity* other);
};


struct BitTracker
{
public:
	static const uint8_t TOTAL_BITS = 64;
	unsigned short bits;
	bool Has(uint8_t index) { return (bits & (1UL << index)) == (1UL << index); };
	void Add(uint8_t index) {
		if (!Has(index)) {
			bits = bits | (1UL << index);
		}
	}
	void Remove(uint8_t index) { if (Has(index)) bits = bits ^ (1UL << index); }
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
	static Entity* CreateEntity();
	static void AddComponentKey(Entity* entity, uint8_t componentKey);// = > activeEntityBits[entity.id].Add(componentKey);
	static void RemoveComponentKey(Entity* entity, uint8_t componentKey);// = > activeEntityBits[entity.id].Remove(componentKey);
	static void DestroyEntity(Entity* entity);
	
};


template <typename T>
bool Entity::HasComponent() {
	return ComponentManager::componentKeys.count(typeid(T).name()) > 0 && ComponentManager::GetComponentSet<T>()->HasEntity(this->_id);
}

template <typename T>
T* Entity::AddComponent() {
	ComponentSet<T>* componentSet = ComponentManager::GetComponentSet<T>();
	if (componentSet != nullptr) {
		if (componentSet->AddEntity(this->_id)) {
			EntityManager::AddComponentKey(this, ComponentManager::GetComponentKey<T>());
			return componentSet->GetComponent(this->_id);
		}
		else {
			std::cerr << "Tried to add a component this entity already has\n";
		}
	}
	else {
		std::cerr << "Try to add component that does not exist in the ComponentManager\n";
	}
}

template <typename T>
void Entity::RemoveComponent() {
	ComponentSet<T>* componentSet = ComponentManager::GetComponentSet<T>();
	if (componentSet != nullptr) {
		if (componentSet->RemoveEntity(this->_id)) {
			EntityManager::RemoveComponentKey(this, componentSet->key);
			//delete componentSet;
		}
		else {
			std::cerr << "Tried to remove a component this entity does not have\n";
		}
	}
	else {
		std::cerr << "Try to remove component that does not exist in the ComponentManager\n";
	}
}

template <typename T>
T* Entity::GetComponent() {
	ComponentSet<T>* component = ComponentManager::GetComponentSet<T>();
	return component->GetComponent(this->_id);
}

template <typename T>
void Entity::SetComponent(T* data) {
	ComponentSet<T>* component = ComponentManager::GetComponentSet<T>();
	component->SetEntityData(this->_id, data);
}
#endif // !ENTITY_MANAGER