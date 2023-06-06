#ifndef _COMPONENTSET_H_
#define _COMPONENTSET_H_
#include "Sets.h"
#include "PackedArray.cpp"
#include <string>
template <class T>
class ComponentSet : public Sets
{
private:
	unsigned short sparseArray[Sets::MAX_ENTITIES] = {};
	PackedArray<unsigned short> entities;
	PackedArray<T> components; 
	unsigned short count = 0;
public:
	ComponentSet();
	bool HasEntity(unsigned short entityID);
	bool RemoveEntity(unsigned short entityID);
	bool AddEntity(unsigned short entityID, T newComponent);
	T GetComponent(unsigned short entityID);
	std::string ComponentName();
};

//int ComponentManager::componentCount = 0;
//PackedArray<Sets> ComponentManager::allComponentSets;
//#include "ComponentSet.cpp"
#endif
