#include "ComponentSet.h"
//#include <iostream>
//#include <typeinfo>
//
//template <class T>
//ComponentSet<T>::ComponentSet()
//{
//
//	std::cout << "Component has been made\n";
//	for (int i = 0; i < Sets::MAX_ENTITIES; i++)
//		this->sparseArray[i] = Sets::MAX_ENTITIES + 1;
//}
//template <class T>
//bool ComponentSet<T>::HasEntity(unsigned short entityID)
//{
//	return entityID < (sizeof(this->sparseArray) / sizeof(this->sparseArray[0]))
//		&& this->sparseArray[entityID] != Sets::MAX_ENTITIES + 1
//		&& this->entities.Count > 0
//		&& this->entities.Get(this->sparseArray[entityID]) == entityID;
//}
//template <class T>
//bool ComponentSet<T>::RemoveEntity(unsigned short entityID)
//{
//	unsigned short pindex = this->sparseArray[entityID];
//	if (this->HasEntity(entityID))
//	{
//		// EntityManager.RemoveComponentKey(entity, ComponentKey<T>.key);
//		unsigned short  swapID = this->entities.Get(this->entities.Count - 1);
//		this->entities.Remove(pindex);
//		this->components.Remove(pindex);
//		this->sparseArray[swapID] = pindex;
//		this->sparseArray[entityID] = -1;
//		this->count = this->entities.Count;
//		return true;
//	}
//	return false;
//}
//template <class T>
//bool ComponentSet<T>::AddEntity(unsigned short  entityID, T newComponent)
//{
//	if (!this->HasEntity(entityID))
//	{
//		// EntityManager.AddComponentKey(newEntity, ComponentKey<T>.key);
//		this->sparseArray[entityID] = this->entities.Count;
//		this->entities.Add(entityID);
//		this->components.Add(newComponent);
//		this->count = this->entities.Count;
//		return true;
//	}
//	return false;
//}
//template <class T>
//T ComponentSet<T>::GetComponent(unsigned short  entityID)
//{
//	return this->components.Get(this->sparseArray[entityID]);
//}
//template <class T>
//std::string ComponentSet<T>::ComponentName() {
//	return typeid(T).name();
//}
