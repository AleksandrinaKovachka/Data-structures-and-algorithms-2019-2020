#pragma once
#include "pch.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm> 

struct Column
{
	std::string nameOfColumn;
	enum Type { INT, STRING } typeOfColumn;
};

class ColumnsInfo
{
	Column* columns;
	unsigned numberOfColumns;
	unsigned maxColumns;

public:
	ColumnsInfo();

	ColumnsInfo(const ColumnsInfo& rhs);

	ColumnsInfo& operator=(const ColumnsInfo& rhs);

	~ColumnsInfo();

	void setMaxColumns(unsigned max);

	unsigned getNumberOfColumns() const;

	void pushColumn(const Column& column);

	Column& operator[](unsigned index);

	const Column& operator[](unsigned index) const;

	void createColumnsInfo(std::string& stringFromFile);

	void print() const;

	void printOnly(const std::string& nameOfColumn) const;
};
