
#ifndef _PACKEDARR_H_
#define _PACKEDARR_H_
template <typename T>
class PackedArray
{
private:
	int size;
	T* arr;
public:
	int Count;
	PackedArray();
	T *Get(int index);
	void Add(const T& item);
	void Remove(int index);
};
//#include "PackedArray.cpp"
#endif