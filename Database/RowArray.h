#pragma once
#include "pch.h"
#include "ColumnType.h"

class RowArray
{
private:
	ColumnType** row;
	unsigned size;
	unsigned capacity;

public:
	RowArray(unsigned capacity);

	RowArray(const RowArray& rhs);

	RowArray& operator=(const RowArray& rhs);

	~RowArray();

	void pushBack(const ColumnType& element);

	ColumnType& operator[](unsigned index);

	const ColumnType& operator[](unsigned index) const;

	void print() const;

	void printOnly(unsigned index) const;
};
