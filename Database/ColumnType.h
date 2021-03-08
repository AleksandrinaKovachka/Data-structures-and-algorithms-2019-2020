#pragma once
#include "pch.h"
#include <iostream>
#include <iomanip>
#include <string>

class ColumnType
{
public:
	virtual std::string getType() = 0;

	virtual ColumnType* clone() const = 0;

	virtual void print() const = 0;

	virtual bool isTrue(std::string& operation, const void* value) const = 0;

	virtual ~ColumnType() {};
};
