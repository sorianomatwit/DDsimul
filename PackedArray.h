
#ifndef _PACKEDARR_H_
#define _PACKEDARR_H_
#include <cstring>
#include <vector>
#include <algorithm>
#include <iostream>

template <typename T>
class PackedArray
{
private:
	T* arr;
	void Resize();
	
public:
	size_t size = 8;
	mutable int Count;
	PackedArray();
	~PackedArray();
	T* Get(int index);
	void Add(const T& item);
	void Remove(int index);

};
template <typename T>
PackedArray<T>::PackedArray()
{
	arr = (T*)malloc(sizeof(T) * size);
	this->Count = 0;
}
template <typename T>
PackedArray<T>::~PackedArray() = default;

template <typename T>
T* PackedArray<T>::Get(int index)
{
	if (index >= 0 && index < Count) {
		return &arr[index];
	}
	return nullptr;
}
template <typename T>

void PackedArray<T>::Add(const T& item)
{
	if (this->size == this->Count) this->Resize();
	//arr.push_back(item);
	arr[Count] = item;
	this->Count++;
}
template<typename T>
void PackedArray<T>::Resize() {
	size_t newSize = size * 2;
	T* newArr = (T*)malloc(sizeof(T) * newSize);

	std::memcpy(newArr, arr, size * sizeof(T));
	size = newSize;
	delete[]arr;
	arr = newArr;
}


template <typename T>
void PackedArray<T>::Remove(int index)
{
	if (index >= 0 && index < Count)
	{
		T temp = arr[Count - 1];
		arr[index] = temp;
		//std::swap(arr[index], arr.back());
		//arr.pop_back();
		Count--;
	}
}
#endif