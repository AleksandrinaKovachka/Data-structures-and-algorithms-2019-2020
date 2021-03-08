#pragma once
#include "pch.h"
#include <fstream>
#include "RowArray.h"
#include "IntType.h"
#include "StringType.h"
#include "ColumnsInfo.h"
#include "DynamicArray.h"
#include "Queue.h"
#include "Stack.h"

class Table
{
private:
	std::string nameOfTable;
	ColumnsInfo columnsInfo;
	DynamicArray<int> arrayInfo;

public:
	Table(const std::string& name);

	void insertInTable(std::string& inputString);

	void selectTable(std::string& inputString, const std::string& nameOfColumn);

	void removeRows(std::string& inputString);

	void compactTable();

	bool haveColumn(const std::string& nameOfColumn) const;

	unsigned getTypeOfColumn(const std::string& nameOfColumn) const;

	unsigned getTypeOfColumnByIndex(unsigned index) const;

	unsigned getNumberOfColumns() const;

private:

	void getInfoForColumns();

	void insertOneRow(std::string& inputString, int indexToAdd);

	void inputInInfoFile(int sizeOfRow);

	int getSizeOfFileInfo();

	void generateInfoArray();

	bool findElementToDelete() const;

	bool isAllDeleted() const;

	int sumOfPreviousIndexInInfoFile(int index) const;

	int movePointerInPosition(int index) const;

	void changeNumberOfColumn(int index);

	void printRow(const RowArray& row);

	void printBy(const RowArray& row, const std::string& nameOfColumn);

	void createQueueForSelect(std::string& inputString, Queue<std::string>& queueForSelect);

	bool haveOrderBy(const std::string& inputString);

	bool haveFunctional(RowArray& row, Queue<std::string>& queue);

	int convertStringToInt(std::string& stringToConvert);

	void inputTable(std::string& inputString, const std::string& nameOfColumn);

	void inputTableCaseOrderBy(std::string& inputString, const std::string& nameOfColumn);

	void saveArrayInInfoFile() const;

	void compactInfoFile();

	void sortArray(DynamicArray<int>& indexArray, DynamicArray<std::string>& arrayToSort, unsigned intOrString);

	void printSortRows(DynamicArray<int>& indexArray, const std::string& nameOfColumn);
};
