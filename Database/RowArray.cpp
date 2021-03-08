#include "pch.h"
#include "RowArray.h"

RowArray::RowArray(unsigned capacity)
{
	size = 0;
	this->capacity = capacity;
	row = new ColumnType*[capacity];
}

RowArray::RowArray(const RowArray& rhs)
{
	size = rhs.size;
	capacity = rhs.capacity;
	row = new ColumnType*[capacity];
	for (unsigned i = 0; i < size; ++i)
	{
		row[i] = rhs.row[i]->clone();
	}
}

RowArray& RowArray::operator=(const RowArray& rhs)
{
	if (this != &rhs)
	{
		delete[] row;
		size = rhs.size;
		capacity = rhs.capacity;
		row = new ColumnType*[capacity];
		for (unsigned i = 0; i < size; ++i)
		{
			row[i] = rhs.row[i]->clone();
		}
	}
	return *this;

}

RowArray::~RowArray()
{
	delete[] row;

}

void RowArray::pushBack(const ColumnType& element)
{
	if (size == capacity)
	{
		return;
	}
	row[size++] = element.clone();
}

ColumnType& RowArray::operator[](unsigned index)
{
	return *row[index];
}

const ColumnType& RowArray::operator[](unsigned index) const
{
	return *row[index];
}

void RowArray::print() const
{
	for (unsigned i = 1; i < size; ++i)
	{
		row[i]->print();
	}
	std::cout << '|';
	std::cout << '\n';
}

void RowArray::printOnly(unsigned index) const
{
	row[index]->print();
	std::cout << '|';
	std::cout << '\n';
}