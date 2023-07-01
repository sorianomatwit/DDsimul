#ifndef _MANAGER_H_
#define _MANAGER_H_
#include "Sets.h"
#include "ComponentSet.h"
#include "SimulatorData.h"
#include <map>
#include <string.h>
#include <functional>
#include <iostream>

class ComponentManager
{
private:

public:
    static std::vector<std::function<bool(unsigned short)>> allEntityRemoves;
    static unsigned short componentCount;
    static PackedArray<Sets*> allComponentSets;
    static std::map<std::string, unsigned short> componentKeys;

    template <typename T>
    static ComponentSet<T>* CreateComponentSet();

    template <typename T>
    static ComponentSet<T>* GetComponentSet();


    template <typename T>
    static unsigned short GetComponentKey();

    template <typename T>
    static void DeleteComponent();

};

template <typename T>
ComponentSet<T>* ComponentManager::CreateComponentSet()
{
    ComponentSet<T>* instanceOfComponentSet = new ComponentSet<T>(ComponentManager::componentCount);
    ComponentManager::allComponentSets.Add(instanceOfComponentSet);
    ComponentManager::componentKeys[typeid(T).name()] = ComponentManager::componentCount;


    ComponentManager::allEntityRemoves.push_back(
        std::bind(&ComponentSet<T>::RemoveEntity, instanceOfComponentSet, std::placeholders::_1)
    );

    ComponentManager::componentCount+=1;
    //std::cout << "Component Created\n" << "Key: " << ComponentManager::componentKeys[typeid(T).name()] << "\nType: " << typeid(T).name() << std::endl;
    return instanceOfComponentSet;
}

template <class T>
ComponentSet<T>* ComponentManager::GetComponentSet()
{
    const char* comp = typeid(T).name();
	auto ptr = *ComponentManager::allComponentSets.Get(ComponentManager::componentKeys[comp]);
    //(reinterpret_cast<ComponentSet<T>*>(ptr)
    ComponentSet<T>* result = reinterpret_cast<ComponentSet<T>*>(ptr);
	return result;
}


template <typename T>
unsigned short ComponentManager::GetComponentKey() {
    return ComponentManager::componentKeys[typeid(T).name()];
}

template <typename T>
void ComponentManager::DeleteComponent() {
    uint8_t key = ComponentManager::GetComponentKey<T>();
    ComponentManager::allComponentSets.Remove(key);

}

#endif