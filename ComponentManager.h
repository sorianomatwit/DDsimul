#ifndef _MANAGER_H_
#define _MANAGER_H_
#include "Sets.h"
#include "ComponentSet.h"
#include "SimulatorData.h"
#include <map>
#include <string.h>
#include <functional>

class ComponentManager
{
private:
    
public :
     static int componentCount;
     static PackedArray<Sets> allComponentSets;
     static std::map<std::string, int> componentKeys;
     static std::vector<std::function<bool(unsigned short)>> allEntityRemoves;

    template <typename T>
    static ComponentSet<T> * CreateComponentSet();

    template <typename T>
    static ComponentSet<T> * GetComponentSet();

    template <typename T>
    static int GetComponentKey();

};

template <typename T>
ComponentSet<T>* ComponentManager::CreateComponentSet()
{
    ComponentSet<T>* instanceOfComponentSet = new ComponentSet<T>(ComponentManager::componentCount);
    ComponentManager::allComponentSets.Add(*instanceOfComponentSet);
    ComponentManager::componentKeys[typeid(T).name()] = ComponentManager::componentCount;

    std::function<bool(unsigned short)> removeEntityFunc = std::bind(&ComponentSet<T>::RemoveEntity, instanceOfComponentSet, std::placeholders::_1);

    ComponentManager::allEntityRemoves.push_back(removeEntityFunc);

    ComponentManager::componentCount++;
    return instanceOfComponentSet;
}

template <class T>
ComponentSet<T>* ComponentManager::GetComponentSet()
{
	Sets* ptr;
	ptr = ComponentManager::allComponentSets.Get(ComponentManager::componentKeys[typeid(T).name()]);
	ComponentSet<T>* result = reinterpret_cast<ComponentSet<T>*>(ptr);
	return result;
}

template <typename T>
int ComponentManager::GetComponentKey() {
    return ComponentManager::componentKeys[typeid(T).name()];
}

#endif