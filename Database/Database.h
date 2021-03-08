#pragma once
#include "pch.h"
#include "Table.h"

const std::string fileWithListTables = "ListTables.txt";

class Database
{
private:
	std::string inputString;
	unsigned sizeOfString;

public:
	Database();

	void selectFunction(std::string& inputString);

private:

	void CreateTable();

	bool checkInfoFromCreateTable();

	void DropTable();

	void ListTable() const;

	void TableInfo() const;

	void Select();

	bool checkInfoFromSelect(const Table& table, const std::string& nameOfColumn, const std::string& nameOfTable);

	void Remove();

	bool checkInfoFromRemove(const Table& table, const std::string& nameOfTable);

	void Insert();

	bool checkInfoFromInsert(const Table& table) const;

	void Compact();

	bool haveTable(const std::string& nameOfTable) const;

	int sizeOfListFile() const;

	int sizeOfInfoFile() const;
};
