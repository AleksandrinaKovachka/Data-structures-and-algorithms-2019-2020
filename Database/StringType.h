#pragma once
#include "pch.h"
#include "ColumnType.h"

class StringType : public ColumnType
{
private:
	std::string stringData;

public:
	StringType(std::string stringData);

	std::string getType();

	StringType* clone() const;

	bool isTrue(std::string& operation, const void* value) const;

	const std::string& getData() const;

	void print() const;
};
