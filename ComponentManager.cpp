
#include "ComponentManager.h"



//template <typename T>
//ComponentSet<T> ComponentManager::CreateComponentSet()
//{
//	ComponentSet<T> instanceOfComponentSet;
//	ComponentManager::allComponentSets.Add(instanceOfComponentSet);
//	ComponentManager::allEntityRemoves.Add(*instanceOfComponentSet.RemoveEntity);
//	ComponentManager::componentKeys[typeid(T).name()] = componentCount;
//	ComponentManager::componentCount++;
//	return instanceOfComponentSet;
//}
//
//template <class T>
//ComponentSet<T>* ComponentManager::GetComponentSet()
//{
//	Sets* ptr;
//	ptr = ComponentManager::allComponentSets.Get(ComponentManager::componentKeys[typeid(T).name()]);
//	ComponentSet<T>* result = reinterpret_cast<ComponentSet<T>*>(ptr);
//	return NULL;
//}
