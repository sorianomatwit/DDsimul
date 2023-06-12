#ifndef _COMPONENTSET_H_
#define _COMPONENTSET_H_
#include "Sets.h"
#include "PackedArray.h"
#include <string>
#include <iostream>
#include <typeinfo>
#include <array>
template <class T>
class ComponentSet : public Sets
{
private:
	std::array<unsigned short, Sets::MAX_ENTITIES> sparseArray;
	PackedArray<unsigned short> entities;
	PackedArray<T> components; 
	
public:
	int key = 0;
	unsigned short count = 0;
	ComponentSet(int key);
	~ComponentSet();
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
	sparseArray.fill(Sets::MAX_ENTITIES + 1);
	
}
template <class T>
ComponentSet<T>::~ComponentSet() = default;

template <class T>
bool ComponentSet<T>::HasEntity(unsigned short entityID)
{
	return entityID < sparseArray.size()
		&& this->sparseArray[entityID] != Sets::MAX_ENTITIES + 1
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
		this->sparseArray[entityID] = Sets::MAX_ENTITIES + 1;
		this->count = this->entities.Count;
		return true;
	}
	return false;
}
template <class T>
bool ComponentSet<T>::AddEntity(unsigned short  entityID)
{
	if (!this->HasEntity(entityID) && entityID < sparseArray.size())
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
#endif
