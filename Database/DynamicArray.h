#pragma once
#include "pch.h"

template <class T>
class DynamicArray
{
private:
	T* arr;
	unsigned size;
	unsigned capacity;

	void copy(const DynamicArray<T>& rhs);
	void resize();
	void erase();

public:
	DynamicArray();
	DynamicArray(const DynamicArray<T>& rhs);
	DynamicArray<T>& operator=(const DynamicArray<T>& rhs);
	~DynamicArray();

	void pushBack(const T& element);
	void removeElement(unsigned index);
	unsigned sizeOfArray() const;

	T& operator[](unsigned index);
	const T& operator[](unsigned index) const;
};

template <class T>
void DynamicArray<T>::copy(const DynamicArray<T>& rhs)
{
	size = rhs.size;
	capacity = rhs.capacity;
	arr = new T[capacity];
	for (unsigned i = 0; i < size; ++i)
	{
		arr[i] = rhs.arr[i];
	}
}

template <class T>
void DynamicArray<T>::resize()
{
	capacity *= 2;
	T* newArr = new T[capacity];
	for (unsigned i = 0; i < size; ++i)
	{
		newArr[i] = arr[i];
	}
	erase();
	arr = newArr;
}

template <class T>
void DynamicArray<T>::erase()
{
	delete[] arr;
}

template <class T>
DynamicArray<T>::DynamicArray()
{
	capacity = 8;
	arr = new T[capacity];
	size = 0;
}

template <class T>
DynamicArray<T>::DynamicArray(const DynamicArray<T>& rhs)
{
	copy(rhs);
}

template <class T>
DynamicArray<T>& DynamicArray<T>::operator=(const DynamicArray<T>& rhs)
{
	if (this != rhs)
	{
		erase();
		copy(rhs);
	}

	return *this;
}

template <class T>
DynamicArray<T>::~DynamicArray()
{
	erase();
}

template <class T>
void DynamicArray<T>::pushBack(const T& element)
{
	if (size >= capacity)
	{
		resize();
	}
	arr[size++] = element;
}

template <class T>
void DynamicArray<T>::removeElement(unsigned index)
{
	for (unsigned i = index; i < size - 1; ++i)
	{
		arr[i] = arr[i + 1];
	}

	size--;
}

template <class T>
unsigned DynamicArray<T>::sizeOfArray() const
{
	return size;
}

template <class T>
T& DynamicArray<T>::operator[](unsigned index)
{
		return arr[index];
}

template <class T>
const T& DynamicArray<T>::operator[](unsigned index) const
{
		return arr[index];
}

