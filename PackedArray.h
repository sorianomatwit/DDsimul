
#ifndef _PACKEDARR_H_
#define _PACKEDARR_H_
#include <cstring>
#include <vector>
#include <algorithm>

template <typename T>
class PackedArray
{
private:
	std::vector<T> arr;
public:
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
	arr.push_back(item);
	this->Count++;
}


template <typename T>
void PackedArray<T>::Remove(int index)
{
	if (index >= 0 && index < Count)
	{
		std::swap(arr[index], arr.back());
		arr.pop_back();
		Count--;
	}
}
#endif