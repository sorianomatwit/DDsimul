#include "PackedArray.h"

//template <typename T>
//PackedArray<T>::PackedArray()
//{
//	this->size = 4;
//	this->arr = new T[this->size];
//	this->Count = 0;
//}
//template <typename T>
//T * PackedArray<T>::Get(int index)
//{
//	return &this->arr[index];
//}
//template <typename T>
//
//void PackedArray<T>:: Add(const T& item)
//{
//	if (this->Count == this->size)
//	{
//		this->size *= 2;
//		T* resizedArray = new T[this->size];
//
//		for (int i = 0; i < this->Count; i++) {
//			resizedArray[i] = arr[i];
//		}
//
//		delete[] arr; // Deallocate the memory of the original array
//
//		arr = resizedArray;
//	}
//	arr[this->Count] = item;
//	this->Count++;
//}
//
//
//template <typename T>
//void PackedArray<T>:: Remove(int index)
//{
//	T lastValue = this->arr[Count - 1];
//	this->arr[index] = lastValue;
//	this->arr[Count - 1] = T();
//	Count--;
//}
//void tempFunc2() {
//	PackedArray<int> notApplical;
//}
