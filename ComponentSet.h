#ifndef _COMPONENTSET_H_
#define _COMPONENTSET_H_
#include "Sets.h"
#include "PackedArray.cpp"
#include <string>
#include <iostream>
#include <typeinfo>
template <class T>
class ComponentSet : public Sets
{
private:
	unsigned short sparseArray[Sets::MAX_ENTITIES] = {};
	PackedArray<unsigned short> entities;
	PackedArray<T> components; 
	
public:
	int key = 0;
	unsigned short count = 0;
	ComponentSet(int key);
	bool HasEntity(unsigned short entityID);
	bool RemoveEntity(unsigned short entityID);
	bool AddEntity(unsigned short entityID);
	T* GetComponent(unsigned short entityID);
	std::string ComponentName();
};


template <class T>
ComponentSet<T>::ComponentSet(int key)
{
	this->key = key;
	std::cout << "Component has been made\n";
	for (int i = 0; i < Sets::MAX_ENTITIES; i++)
		this->sparseArray[i] = Sets::MAX_ENTITIES + 1;
}
template <class T>
bool ComponentSet<T>::HasEntity(unsigned short entityID)
{
	return entityID < (sizeof(this->sparseArray) / sizeof(this->sparseArray[0]))
		&& this->sparseArray[entityID] != Sets::MAX_ENTITIES + 1
		&& this->entities.Count > 0
		&& *this->entities.Get(this->sparseArray[entityID]) == entityID;
}
template <class T>
bool ComponentSet<T>::RemoveEntity(unsigned short entityID)
{
	unsigned short pindex = this->sparseArray[entityID];
	if (this->HasEntity(entityID))
	{
		unsigned short* swapID = this->entities.Get(this->entities.Count - 1);
		this->entities.Remove(pindex);
		this->components.Remove(pindex);
		this->sparseArray[*swapID] = pindex;
		this->sparseArray[entityID] = -1;
		this->count = this->entities.Count;
		return true;
	}
	return false;
}
template <class T>
bool ComponentSet<T>::AddEntity(unsigned short  entityID)
{
	if (!this->HasEntity(entityID))
	{
		this->sparseArray[entityID] = this->entities.Count;
		this->entities.Add(entityID);
		this->components.Add(T());
		this->count = this->entities.Count;
		return true;
	}
	return false;
}
template <class T>
T* ComponentSet<T>::GetComponent(unsigned short entityID)
{
	if (this->HasEntity(entityID)) {
		return this->components.Get(this->sparseArray[entityID]);
	} 
	return nullptr;
}
template <class T>
std::string ComponentSet<T>::ComponentName() {
	return typeid(T).name();
}
//int ComponentManager::componentCount = 0;
//PackedArray<Sets> ComponentManager::allComponentSets;
//#include "ComponentSet.cpp"
#endif
