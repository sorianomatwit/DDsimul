#include "Entity.h"

Entity::Entity(unsigned short id) {
	this->_id = id;
}
template <typename T>
bool Entity::HasComponent() {
	ComponentSet<T>* componentSet = ComponentManager::GetComponentSet<T>();
	return componentSet->HasEntity(this->_id);
 }

template <typename T>
void Entity::AddComponent(T component) {
	ComponentSet<T>* componentSet = ComponentManager::GetComponentSet<T>();
	return componentSet->AddEntity(this->_id);
}

template <typename T>
void Entity::RemoveComponent() {
	ComponentSet<T>* componentSet = ComponentManager::GetComponentSet<T>();
	return componentSet->RemoveEntity(this->_id);
}

template <typename T>
T Entity::GetComponent() {
	ComponentSet<T>* componentSet = ComponentManager::GetComponentSet<T>();
	return componentSet->GetComponent(this->_id);
}

bool Entity::Equals(Entity* other) {
	return this->_id == other->_id;
}