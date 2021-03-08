#pragma once
#include "pch.h"
#include "ColumnType.h"

class IntType : public ColumnType
{
private:
	int data;

public:
	IntType(int data);

	std::string getType();

	IntType* clone() const;

	bool isTrue(std::string& operation, const void* value) const;

	int getData() const;

	void print() const;
};
