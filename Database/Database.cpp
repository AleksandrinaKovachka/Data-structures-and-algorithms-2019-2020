#include "pch.h"
#include "DataBase.h"

Database::Database()
{
	inputString = "";
	sizeOfString = 0;
	
	std::fstream file(fileWithListTables, std::ios::app);
	file.close();
}

void Database::selectFunction(std::string& input)
{
	inputString = input;
	sizeOfString = inputString.length();
	std::string nameOfFunction;
	for (unsigned i = 0; i < sizeOfString; ++i)
	{
		if (inputString[i] == ' ')
		{
			break;
		}
		nameOfFunction += inputString[i];
	}

	inputString.erase(0, nameOfFunction.length() + 1);
	sizeOfString -= nameOfFunction.length() + 1;

	if (nameOfFunction == "CreateTable")
	{
		if (inputString == "")
		{
			std::cout << "CreateTable have table name and list of information\n";
			return;
		}
		CreateTable();
	}
	else if (nameOfFunction == "DropTable")
	{
		if (inputString == "")
		{
			std::cout << "DropTable have table name\n";
			return;
		}
		DropTable();
	}
	else if (nameOfFunction == "ListTables")
	{
		if (inputString != "")
		{
			std::cout << "Wrog input - function \"ListTable\" have not arguments!\n";
			return;
		}
		ListTable();
	}
	else if (nameOfFunction == "TableInfo")
	{
		if (inputString == "")
		{
			std::cout << "TableInfo have table name\n";
			return;
		}
		TableInfo();
	}
	else if (nameOfFunction == "Select")
	{
		if (inputString == "")
		{
			std::cout << "Select have list of arguments\n";
			return;
		}
		Select();
	}
	else if (nameOfFunction == "Remove")
	{
		if (inputString == "")
		{
			std::cout << "Remove have list of arguments\n";
			return;
		}
		Remove();
	}
	else if (nameOfFunction == "Insert")
	{
		if (inputString == "")
		{
			std::cout << "Insert have table name and data for column in table\n";
			return;
		}
		Insert();
	}
	else if (nameOfFunction == "Compact")
	{
		Compact();
	}
	else
	{
		std::cout << "Wrong input - the input function is not exist!\n";
	}
}

void Database::CreateTable()
{
	std::string nameOfTable;
	for (unsigned i = 0; i < sizeOfString; ++i)
	{
		if (inputString[i] == ' ')
		{
			break;
		}
		nameOfTable += inputString[i];
	}

	inputString.erase(0, nameOfTable.length() + 1);
	sizeOfString -= nameOfTable.length() + 1;

	if (sizeOfListFile() > 0)
	{
		if (haveTable(nameOfTable))
			std::cout << "You can not create table with same names\n";
		return;
	}

	if (checkInfoFromCreateTable() == false)
	{
		std::cout << "Wrong list of name and type\n";
		return;
	}

	//Добавя името на таблицата във файла с имената на таблиците
	std::ofstream fileList(fileWithListTables, std::ios::app);
	if (fileList.is_open())
	{
		fileList << nameOfTable << '\n';
		fileList.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	//Създава файл с името на таблицата .tbl и записва информацията за колоните
	nameOfTable += ".tbl";
	std::ofstream fileToCreateTable(nameOfTable, std::ios::binary | std::ios::app);
	if (fileToCreateTable.is_open())
	{
		fileToCreateTable.write((const char*)&sizeOfString, sizeof(unsigned));
		fileToCreateTable.write(inputString.c_str(), sizeOfString);

		fileToCreateTable.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	//Създава файл с името на таблицата .info за дължините на всеки ред
	unsigned sizeOfName = nameOfTable.length();
	nameOfTable.erase(sizeOfName - 3, sizeOfName - 1);
	nameOfTable += "info";

	int sizeToInfoFile = sizeOfString + sizeof(unsigned);
	std::ofstream fileWithInfo(nameOfTable, std::ios::binary | std::ios::app);
	if (fileWithInfo.is_open())
	{
		fileWithInfo.write((const char*)&sizeToInfoFile, sizeof(int));

		fileWithInfo.close();
	}
	else
	{
		std::cout << "Unable to open\n";
	}

	nameOfTable.erase(sizeOfName - 4, sizeOfName - 1);
	std::cout << "Table " << nameOfTable << " created!\n";
}

bool Database::checkInfoFromCreateTable()
{
	if (inputString[0] != '(' && inputString[sizeOfString - 1] != ')')
	{
		return false;
	}
	std::string addElement;
	for (unsigned i = 1; i < sizeOfString - 1; ++i)
	{
		if (inputString[i] == ':')
		{
			if (addElement == "Int" || addElement == "String")
			{
				return false;
			}
			addElement = "";
			continue;
		}
		else if (inputString[i] == ',' && inputString[i + 1] == ' ')
		{
			if (addElement != "Int" && addElement != "String")
			{
				return false;
			}
			addElement = "";
			++i;
			continue;
		}
		else
		{
			addElement += inputString[i];
		}
	}

	if (addElement != "Int" && addElement != "String")
	{
		return false;
	}
	return true;
}

void Database::DropTable()
{
	std::string nameOfTable;
	for (unsigned i = 0; i < sizeOfString; ++i)
	{
		if (inputString[i] == ' ')
		{
			break;
		}
		nameOfTable += inputString[i];
	}

	inputString.erase(0, nameOfTable.length());
	sizeOfString -= nameOfTable.length();

	if (sizeOfString != 0)
	{
		std::cout << "Function \"DropTable\" have only name of table\n";
		return;
	}
	else if (haveTable(nameOfTable) == false || sizeOfListFile() <= 0)
	{
		std::cout << "Can not delete table that not exist\n";
		return;
	}

	const char* temp = "tempFile.txt";
	std::ofstream tempFile(temp, std::ios::app);
	if (!tempFile.is_open())
	{
		std::cout << "Unable to open file\n";
		return;
	}

	std::ifstream fileWithList(fileWithListTables);
	if (!fileWithList.is_open())
	{
		std::cout << "Unable to open file\n";
		tempFile.close();
		return;
	}

	std::string line;
	while (getline(fileWithList, line))
	{
		if (line != nameOfTable)
		{
			tempFile << line << '\n';
		}
	}

	tempFile.close();
	fileWithList.close();

	const char* nameToDelete = fileWithListTables.c_str();

	std::remove(nameToDelete);
	std::rename(temp, nameToDelete);

	std::cout << "Table " << nameOfTable << " is delete\n";

	std::string table = nameOfTable + ".tbl";
	const char* tblDelete = table.c_str();
	std::remove(tblDelete);

	table = nameOfTable + ".info";
	const char* infoDelete = table.c_str();
	std::remove(infoDelete);
}

void Database::ListTable() const
{
	unsigned sizeOfFile = sizeOfListFile();
	if (sizeOfFile == 0)
	{
		std::cout << "There is no tables in the database\n";
		return;
	}
	else if (sizeOfFile == 1)
	{
		std::cout << "There is 1 table in the database:\n";
	}
	else
	{
		std::cout << "There are " << sizeOfFile << " tables in the database:\n";
	}

	std::ifstream fileList(fileWithListTables);
	std::string line;
	if (fileList.is_open())
	{
		while (getline(fileList, line))
		{
			std::cout << line << '\n';
		}

		fileList.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}
}

void Database::TableInfo() const
{
	if (!haveTable(inputString))
	{
		std::cout << "There is no such table!\n";
		return;
	}

	std::string name = inputString + ".tbl";
	std::ifstream file(name, std::ios::binary);
	if (file.is_open())
	{
		unsigned sizeOfString = 0;
		std::string stringFromFile;
		file.read((char*)&sizeOfString, sizeof(unsigned));
		char* buffer = new char[sizeOfString + 1];
		file.read(buffer, sizeOfString);
		stringFromFile.append(buffer, sizeOfString);
		delete[] buffer;

		std::cout << "Table " << inputString << " : " << stringFromFile << '\n';
		file.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
		return;
	}

	name = "";
	name = inputString + ".info";
	std::ifstream infoFile(name, std::ios::binary);
	if (infoFile.is_open())
	{
		unsigned rows = 0;
		int data = 0, temp = 0;
		int sizeOfFile = sizeOfInfoFile();
		for(int i = 0; i < sizeOfFile; ++i)
		{
			temp = 0;
			infoFile.read((char*)&temp, sizeof(int));
			//информацията за колоните също е ред от таблицата
			if (temp > 0)
			{
				data += temp;
				++rows;
			}
		}
		infoFile.close();
		std::cout << "Total " << rows << " rows (" << data << " KB data) in the table\n";
	}
	else
	{
		std::cout << "Unable to open file\n";
	}
}

void Database::Select()
{
	std::string nameOfColumn;
	for (unsigned i = 0; i < sizeOfString; ++i)
	{
		if (inputString[i] == ' ')
		{
			break;
		}
		nameOfColumn += inputString[i];
	}
	inputString.erase(0, nameOfColumn.length() + 1);
	sizeOfString -= nameOfColumn.length() + 1;

	std::string check;
	check.append(inputString, 0, 4);

	if (check != "FROM")
	{
		std::cout << "Wrong input of select function - missing FROM after name of column.\n";
		return;
	}

	inputString.erase(0, 5);
	sizeOfString -= 5;

	std::string nameOfTable;
	for (unsigned i = 0; i < sizeOfString; ++i)
	{
		if (inputString[i] == ' ')
		{
			break;
		}
		nameOfTable += inputString[i];
	}

	inputString.erase(0, nameOfTable.length() + 1);
	sizeOfString -= nameOfTable.length() + 1;

	check = "";
	check.append(inputString, 0, 5);

	if (check != "WHERE")
	{
		std::cout << "Wrong input of select function - missing WHERE after name of table.\n";
		return;
	}

	inputString.erase(0, 6);
	sizeOfString -= 6;

	if (haveTable(nameOfTable))
	{
		Table table(nameOfTable);

		if (checkInfoFromSelect(table, nameOfColumn, nameOfTable))
		{
			table.selectTable(inputString, nameOfColumn);
		}
	}
}

bool Database::checkInfoFromSelect(const Table& table, const std::string& nameOfColumn, const std::string& nameOfTable)
{
	if (nameOfColumn != "*" && !table.haveColumn(nameOfColumn))
	{
		std::cout << nameOfColumn << " is not name of column in table " << nameOfTable << ".\n";
		return false;
	}

	inputString += ' ';
	++sizeOfString;
	std::string partOfString;
	unsigned position = 1;
	unsigned typeOfColumn;
	Stack<char> stackForBrackets;
	for (unsigned i = 0; i < sizeOfString; ++i)
	{
		if (inputString[i] == '(')
		{
			stackForBrackets.push('(');
			continue;
		}
		else if (inputString[i] == ')')
		{
			if (!stackForBrackets.isEmpty())
			{
				if (stackForBrackets.top() != '(')
				{
					std::cout << "Wrong input - not correct brackets.\n";
					return false;
				}
				else
				{
					stackForBrackets.pop();
					continue;
				}
			}
			else
			{
				std::cout << "Wrong input - not correct brackets.\n";
				return false;
			}
		}
		else if (inputString[i] == ' ')
		{
			switch (position)
			{
			case 1:
			{
				if (!table.haveColumn(partOfString))
				{
					std::cout << "Column - " << partOfString << " is not column of table " << nameOfTable << ".\n";
					return false;
				}
				typeOfColumn = table.getTypeOfColumn(partOfString);
				partOfString = "";
				++position;
				break;
			}
			case 2:
			{
				if (partOfString != "!=" && partOfString != "==" && partOfString != ">"
					&& partOfString != ">=" && partOfString != "<" && partOfString != "<=")
				{
					std::cout << "Wrong input of operator " << partOfString << ".\n";
					return false;
				}
				partOfString = "";
				++position;
				break;
			}
			case 3:
			{
				if ((typeOfColumn == 1 && partOfString[0] == '\"') || (typeOfColumn == 2 && partOfString[0] != '\"'))
				{
					std::cout << "Wrong type of column.\n";
					return false;
				}
				partOfString = "";
				++position;
				break;
			}
			case 4:
			{
				if (partOfString != "And" && partOfString != "Or" && partOfString != "ORDER")
				{
					std::cout << "Wrong input - not correct use of And, Or or ORDER BY.\n";
					return false;
				}
				else if (partOfString == "ORDER")
				{
					if (inputString[i + 1] == 'B' && inputString[i + 2] == 'Y')
					{
						partOfString = "";
						i += 4;
						while (i < sizeOfString - 1)
						{
							partOfString += inputString[i];
							++i;
						}
						if (!table.haveColumn(partOfString))
						{
							std::cout << "The column " << partOfString << " that you want to sort table " << nameOfTable << " is not exist.\n";
							return false;
						}
						partOfString = "";
						break;
					}
					else
					{
						std::cout << "Wrong input - not correct use of ORDER BY.\n";
						return false;
					}
				}
				partOfString = "";
				position = 1;
			}
			}
		}
		else
		{
			partOfString += inputString[i];
		}
	}

	if (!stackForBrackets.isEmpty())
	{
		std::cout<< "Wrong input - not correct brackets.\n";
		return false;
	}

	inputString.erase(sizeOfString - 1);
	--sizeOfString;
	return true;
}

void Database::Remove()
{
	std::string check;
	check.append(inputString, 0, 4);

	if (check != "FROM")
	{
		std::cout << "Wrong input of remove function - missing FROM after name of function.\n";
		return;
	}

	inputString.erase(0, 5);
	sizeOfString -= 5;

	std::string nameOfTable;
	for (unsigned i = 0; i < sizeOfString; ++i)
	{
		if (inputString[i] == ' ')
		{
			break;
		}
		nameOfTable += inputString[i];
	}

	inputString.erase(0, nameOfTable.length() + 1);
	sizeOfString -= nameOfTable.length() + 1;

	check = "";
	check.append(inputString, 0, 5);

	if (check != "WHERE")
	{
		std::cout << "Wrong input of remove function - missing WHERE after name of table.\n";
		return;
	}

	inputString.erase(0, 6);
	sizeOfString -= 6;

	if (haveTable(nameOfTable))
	{
		Table table(nameOfTable);
		if (checkInfoFromRemove(table, nameOfTable))
		{
			table.removeRows(inputString);
		}
	}
}

bool Database::checkInfoFromRemove(const Table& table, const std::string& nameOfTable)
{
	inputString += ' ';
	++sizeOfString;
	std::string partOfString;
	unsigned position = 1;
	unsigned typeOfColumn;
	Stack<char> stackForBrackets;
	for (unsigned i = 0; i < sizeOfString; ++i)
	{
		if (inputString[i] == '(')
		{
			stackForBrackets.push('(');
			continue;
		}
		else if (inputString[i] == ')')
		{
			if (!stackForBrackets.isEmpty())
			{
				if (stackForBrackets.top() != '(')
				{
					std::cout << "Wrong input - not correct brackets.\n";
					return false;
				}
				else
				{
					stackForBrackets.pop();
					continue;
				}
			}
			else
			{
				std::cout << "Wrong input - not correct brackets.\n";
				return false;
			}
		}
		else if (inputString[i] == ' ')
		{
			switch (position)
			{
			case 1:
			{
				if (!table.haveColumn(partOfString))
				{
					std::cout << "Column - " << partOfString << " is not column of table " << nameOfTable << ".\n";
					return false;
				}
				typeOfColumn = table.getTypeOfColumn(partOfString);
				partOfString = "";
				++position;
				break;
			}
			case 2:
			{
				if (partOfString != "!=" && partOfString != "==" && partOfString != ">"
					&& partOfString != ">=" && partOfString != "<" && partOfString != "<=")
				{
					std::cout << "Wrong input of operator " << partOfString << ".\n";
					return false;
				}
				partOfString = "";
				++position;
				break;
			}
			case 3:
			{
				if ((typeOfColumn == 1 && partOfString[0] == '\"') || (typeOfColumn == 2 && partOfString[0] != '\"'))
				{
					std::cout << "Wrong type of column.\n";
					return false;
				}
				partOfString = "";
				++position;
				break;
			}
			case 4:
			{
				if (partOfString != "And" && partOfString != "Or")
				{
					std::cout << "Wrong input - not correct use of And or Or.\n";
					return false;
				}
				
				partOfString = "";
				position = 1;
				break;
			}
			}
		}
		else
		{
			partOfString += inputString[i];
		}
	}

	if (!stackForBrackets.isEmpty())
	{
		std::cout << "Wrong input - not correct brackets.\n";
		return false;
	}

	inputString.erase(sizeOfString - 1);
	--sizeOfString;
	return true;
}

void Database::Insert()
{
	std::string check;
	for (unsigned i = 0; i < sizeOfString; ++i)
	{
		if (inputString[i] == ' ')
		{
			break;
		}
		check += inputString[i];
	}

	inputString.erase(0, check.length() + 1);
	sizeOfString -= check.length() + 1;

	if (check != "INTO")
	{
		std::cout << "Wrong input of function insert\n";
	}

	sizeOfString -= check.length() - 1;

	std::string nameOfTable;
	for (unsigned i = 0; i < sizeOfString; ++i)
	{
		if (inputString[i] == ' ')
		{
			break;
		}
		nameOfTable += inputString[i];
	}

	inputString.erase(0, nameOfTable.length() + 1);
	sizeOfString -= nameOfTable.length() + 1;

	if (haveTable(nameOfTable))
	{
		Table table(nameOfTable);
		if (checkInfoFromInsert(table))
		{
			table.insertInTable(inputString);
		}
	}
}

bool Database::checkInfoFromInsert(const Table& table) const
{
	if (inputString[0] != '{' && inputString[sizeOfString - 1] != '}')
	{
		std::cout << "Wrong input - not correct use of { or }.\n";
		return false;
	}

	unsigned numberOfColumn = table.getNumberOfColumns();
	std::string partOfString;
	for (unsigned i = 1; i < sizeOfString - 1; ++i)
	{
		if (inputString[i] == '(')
		{
			++i;
			for (unsigned j = 1; j < numberOfColumn; ++i)
			{
				if (inputString[i] == ',' || inputString[i] == ')')
				{
					if (table.getTypeOfColumnByIndex(j) == 1)
					{
						if (partOfString[0] == '\"')
						{
							std::cout << "Wrong input - the type of column is not the same.\n";
							return false;
						}
						partOfString = "";
						++j;
						++i;
						continue;
					}
					else
					{
						if (partOfString[0] != '\"')
						{
							std::cout << "Wrong input - the type of column is not the same.\n";
							return false;
						}
						partOfString = "";
						++j;
						++i;
						continue;
					}

				}
				partOfString += inputString[i];
			}
			++i;
		}
	}

	return true;
}

void Database::Compact()
{
	std::string nameOfTable;
	for (unsigned i = 0; i < sizeOfString; ++i)
	{
		if (inputString[i] == ' ')
		{
			break;
		}
		nameOfTable += inputString[i];
	}

	inputString.erase(0, nameOfTable.length());

	if (haveTable(nameOfTable))
	{
		Table table(nameOfTable);
		table.compactTable();
	}
}

bool Database::haveTable(const std::string& nameOfTable) const
{
	std::ifstream fileList(fileWithListTables);
	std::string line;
	if (fileList.is_open())
	{
		while (getline(fileList, line))
		{
			if (line == nameOfTable)
			{
				return true;
			}
		}
		fileList.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	return false;
}

int Database::sizeOfListFile() const
{
	int sizeOfFile = 0;
	std::ifstream fileList(fileWithListTables);
	std::string line;
	if (fileList.is_open())
	{
		while (getline(fileList, line))
		{
			++sizeOfFile;
		}
		fileList.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	return sizeOfFile;
}

int Database::sizeOfInfoFile() const
{
	std::string nameOfTableInfo = inputString + ".info";
	std::ofstream fileWithInfo(nameOfTableInfo, std::ios::binary | std::ios::app);
	if (fileWithInfo.is_open())
	{
		fileWithInfo.seekp(0, std::ios::end);
		std::streampos sizeOfFile = fileWithInfo.tellp();
		fileWithInfo.seekp(0, std::ios::beg);
		std::streampos size = sizeOfFile / sizeof(int);

		fileWithInfo.close();

		return (int)size;
	}
	else
	{
		std::cout << "Unable to open\n";
		return 0;
	}
}
