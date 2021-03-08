#pragma once

template <class T>
class DynamicArray
{
private:
	T* arr;
	size_t size;
	size_t capacity;

	void copy(const DynamicArray<T>& rhs);
	void resize();
	void erase();

public:
	DynamicArray();
	DynamicArray(const DynamicArray<T>& rhs);
	DynamicArray<T>& operator=(const DynamicArray<T>& rhs);
	~DynamicArray();

	void push(const T& element);
	void removeElement(size_t index);
	size_t sizeOfArray() const;

	T& operator[](size_t index);
	const T& operator[](size_t index) const;
};

template <class T>
void DynamicArray<T>::copy(const DynamicArray<T>& rhs)
{
	this->size = rhs.size;
	this->capacity = rhs.capacity;
	this->arr = new (std::nothrow) T[this->capacity];
	for (size_t i = 0; i < this->size; ++i)
	{
		this->arr[i] = rhs.arr[i];
	}
}

template <class T>
void DynamicArray<T>::resize()
{
	this->capacity *= 2;
	T* newArr = new (std::nothrow) T[this->capacity];
	for (size_t i = 0; i < this->size; ++i)
	{
		newArr[i] = this->arr[i];
	}
	this->erase();
	this->arr = newArr;
}

template <class T>
void DynamicArray<T>::erase()
{
	delete[] arr;
}

template <class T>
DynamicArray<T>::DynamicArray()
{
	this->capacity = 16;
	this->arr = new (std::nothrow) T[this->capacity];
	this->size = 0;
}

template <class T>
DynamicArray<T>::DynamicArray(const DynamicArray<T>& rhs)
{
	this->copy(rhs);
}

template <class T>
DynamicArray<T>& DynamicArray<T>::operator=(const DynamicArray<T>& rhs)
{
	if (this != rhs)
	{
		this->erase();
		this->copy(rhs);
	}

	return *this;
}

template <class T>
DynamicArray<T>::~DynamicArray()
{
	this->erase();
}

template <class T>
void DynamicArray<T>::push(const T& element)
{
	if (this->capacity < (this->size + 1))
	{
		this->resize();
	}
	this->arr[size++] = element;
}

template <class T>
void DynamicArray<T>::removeElement(size_t index)
{
	for (size_t i = 0; i < this->size; ++i)
	{
		this->arr[i] = this->arr[i + 1];
	}

	this->size--;
}

template <class T>
size_t DynamicArray<T>::sizeOfArray() const
{
	return this->size;
}

template <class T>
T& DynamicArray<T>::operator[](size_t index)
{
	return this->arr[index];
}

template <class T>
const T& DynamicArray<T>::operator[](size_t index) const
{
	return this->arr[index];
}
