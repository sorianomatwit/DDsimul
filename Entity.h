#pragma once
#include "ComponentManager.cpp"

struct Entity
{
public:
	unsigned short _id;
	Entity(unsigned short newId);

	template <typename T>
	bool HasComponent();// { return ComponentManager::GetComponentSet<T>() }//.HasEntity(this);
	template <typename T>
	void AddComponent(T newComponent);// { ComponentManager.GetComponentSet<T>().AddEntity(this, newComponent) };
	template <typename T>
	void RemoveComponent();// { ComponentManager::GetComponentSet<T>().RemoveEntity(this) };
	template <typename T> 
	T GetComponent(); //{ ComponentManager.GetComponentSet<T>().GetComponent(this) };
	void Destroy();
	bool Equals(struct Entity* other);

};

