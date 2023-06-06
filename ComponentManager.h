#ifndef _MANAGER_H_
#define _MANAGER_H_
#include "Sets.h"
#include "ComponentSet.h"
#include "SimulatorData.h"
#include <map>
#include <string.h>
#include <functional>
using RemoveEntity = std::function<bool(unsigned short)>;

class ComponentManager
{
private:
    
public :
     static int componentCount;
     static PackedArray<Sets> allComponentSets;
     static std::map<std::string, int> componentKeys;
     static PackedArray<RemoveEntity> allEntityRemoves;

    template <typename T>
    static ComponentSet<T> CreateComponentSet();

    template <typename T>
    static ComponentSet<T> * GetComponentSet();

};

template <typename T>
ComponentSet<T> ComponentManager::CreateComponentSet()
{
	ComponentSet<T> instanceOfComponentSet;
	ComponentManager::allComponentSets.Add(instanceOfComponentSet);
	ComponentManager::allEntityRemoves.Add(*instanceOfComponentSet.RemoveEntity);
	ComponentManager::componentKeys[typeid(T).name()] = componentCount;
	ComponentManager::componentCount++;
	return instanceOfComponentSet;
}

template <class T>
ComponentSet<T>* ComponentManager::GetComponentSet()
{
	Sets* ptr;
	ptr = ComponentManager::allComponentSets.Get(ComponentManager::componentKeys[typeid(T).name()]);
	ComponentSet<T>* result = reinterpret_cast<ComponentSet<T>*>(ptr);
	return NULL;
}
//int ComponentManager::componentCount = 0;
//PackedArray<Sets> ComponentManager::allComponentSets;
//std::map<std::string, int> ComponentManager::componentKeys;
//PackedArray<RemoveEntity> ComponentManager::allEntityRemoves;
#endif