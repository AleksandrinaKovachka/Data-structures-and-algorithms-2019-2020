#include "pch.h"
#include "Table.h"

Table::Table(const std::string& name)
{
	nameOfTable = name;
	getInfoForColumns();
}


void Table::insertInTable(std::string& inputString)
{
	unsigned printResultOfFunction = 0;
	int newRowIndex = getSizeOfFileInfo();
	unsigned sizeOfString = inputString.length();
	std::string row;
	for (unsigned i = 2; i < sizeOfString - 1; ++i) //започва от 2 защото има проверка и със сигурност преди 2 стоят {(
	{
		if (inputString[i] == ')')
		{
			insertOneRow(row, newRowIndex);
			++printResultOfFunction;
			++newRowIndex;
			row = "";
			if (inputString[i + 1] == ',')
			{
				i += 3;
				continue;
			}
		}
		else
		{
			row += inputString[i];
		}
	}

	std::cout << printResultOfFunction << " rows inserted.\n";
}

void Table::selectTable(std::string& inputString, const std::string& nameOfColumn)
{
	if (haveOrderBy(inputString))
	{
		inputTableCaseOrderBy(inputString, nameOfColumn);
	}
	else
	{
		inputTable(inputString, nameOfColumn);
	}
}

void Table::removeRows(std::string& inputString)
{
	inputTable(inputString, "Remove");
	saveArrayInInfoFile();
}

void Table::compactTable()
{
	generateInfoArray();
	if (!findElementToDelete())
	{
		return;
	}

	const char* newTable = "temp.tbl";
	std::ofstream tempFile(newTable, std::ios::binary | std::ios::app);
	if (!tempFile.is_open())
	{
		std::cout << "Unable to open file\n";
		return;
	}

	std::string nameOfFileTable = nameOfTable + ".tbl";
	std::ifstream fileWithTable(nameOfFileTable, std::ios::binary);
	if (!fileWithTable.is_open())
	{
		std::cout << "Unable to open file\n";
		return;
	}


	//Запис на информацията за колоните
	unsigned sizeOfString = 0;
	std::string stringFromFile;
	fileWithTable.read((char*)&sizeOfString, sizeof(unsigned));
	char* buffer = new char[sizeOfString + 1];
	fileWithTable.read(buffer, sizeOfString);
	stringFromFile.append(buffer, sizeOfString);
	delete[] buffer;

	tempFile.write((const char*)&sizeOfString, sizeof(unsigned));
	tempFile.write(stringFromFile.c_str(), sizeOfString);

	unsigned sizeOfArray = arrayInfo.sizeOfArray();

	unsigned numberOfColumn = columnsInfo.getNumberOfColumns();

	int addToFileInt = 0;

	int newIndexToRow = 1;

	for (unsigned i = 1; i < sizeOfArray; ++i)
	{
		if (arrayInfo[i] > 0)
		{
			tempFile.write((const char*)&newIndexToRow, sizeof(int));
			fileWithTable.seekg(sizeof(int), std::ios::cur);

			for (unsigned j = 1; j < numberOfColumn; ++j)
			{
				if (columnsInfo[j].typeOfColumn == Column::Type::INT)
				{
					addToFileInt = 0;
					fileWithTable.read((char*)&addToFileInt, sizeof(int));
					tempFile.write((const char*)&addToFileInt, sizeof(int));
				}
				else
				{
					sizeOfString = 0;
					stringFromFile = "";
					fileWithTable.read((char*)&sizeOfString, sizeof(unsigned));
					char* buffer = new char[sizeOfString + 1];
					fileWithTable.read(buffer, sizeOfString);
					stringFromFile.append(buffer, sizeOfString);
					delete[] buffer;

					tempFile.write((const char*)&sizeOfString, sizeof(unsigned));
					tempFile.write(stringFromFile.c_str(), sizeOfString);
				}
			}

			++newIndexToRow;
		}
		else
		{
			fileWithTable.seekg(sumOfPreviousIndexInInfoFile(i), std::ios::beg);
		}
	}

	tempFile.close();
	fileWithTable.close();
	const char* name = nameOfFileTable.c_str();
	std::remove(name);
	std::rename(newTable, name);

	compactInfoFile();
}

void Table::getInfoForColumns()
{
	std::string nameOfTableInDirectory = nameOfTable + ".tbl";
	std::ifstream file(nameOfTableInDirectory, std::ios::binary);
	unsigned sizeOfString = 0;
	std::string stringFromFile;
	if (file.is_open())
	{
		file.read((char*)&sizeOfString, sizeof(unsigned));
		char* buffer = new char[sizeOfString + 1];
		file.read(buffer, sizeOfString);
		stringFromFile.append(buffer, sizeOfString);
		delete[] buffer;
		file.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	unsigned max = 2;
	for (unsigned i = 0; i < sizeOfString; ++i)
	{
		if (stringFromFile[i] == ' ')
		{
			++max;
		}
	}

	columnsInfo.setMaxColumns(max);
	columnsInfo.createColumnsInfo(stringFromFile);
}

bool Table::haveColumn(const std::string& nameOfColumn) const
{
	unsigned sizeOfColumn = columnsInfo.getNumberOfColumns();
	for (unsigned i = 0; i < sizeOfColumn; ++i)
	{
		if (columnsInfo[i].nameOfColumn == nameOfColumn)
		{
			return true;
		}
	}

	return false;
}

unsigned Table::getTypeOfColumn(const std::string& nameOfColumn) const //Връща 1 за int и 2 за стринг
{
	unsigned sizeOfColumn = columnsInfo.getNumberOfColumns();
	for (unsigned i = 0; i < sizeOfColumn; ++i)
	{
		if (columnsInfo[i].nameOfColumn == nameOfColumn)
		{
			if (columnsInfo[i].typeOfColumn == Column::Type::INT)
			{
				return 1;
			}
			return 2;
		}
	}
}

unsigned Table::getTypeOfColumnByIndex(unsigned index) const
{
	if (columnsInfo[index].typeOfColumn == Column::Type::INT)
	{
		return 1;
	}

	return 2;
}

unsigned Table::getNumberOfColumns() const
{
	return columnsInfo.getNumberOfColumns();
}

void Table::insertOneRow(std::string& inputString, int indexToAdd)
{
	int sizeOfRow = 0;

	std::string nameOfTableInDirectory = nameOfTable + ".tbl";
	std::ofstream file(nameOfTableInDirectory, std::ios::binary | std::ios::app);
	if (!file.is_open())
	{
		std::cout << "Unable to open\n";
		return;
	}

	//Запис на индекса на реда
	file.write((const char*)&indexToAdd, sizeof(int));
	sizeOfRow += sizeof(int);

	unsigned sizeOfString = inputString.length();
	unsigned numberOfColumns = columnsInfo.getNumberOfColumns();
	unsigned index = 1;
	int addToFileInt = 0;
	std::string addToFileString;
	for (unsigned i = 0; i < sizeOfString && index < numberOfColumns; ++i)
	{
		if (columnsInfo[index].typeOfColumn == Column::Type::INT)
		{
			addToFileInt += inputString[i] - '0';
			if (i + 1 < sizeOfString && inputString[i + 1] != ',')
			{
				addToFileInt *= 10;
				continue;
			}
			else
			{
				file.write((const char*)&addToFileInt, sizeof(int));
				sizeOfRow += sizeof(int);
				addToFileInt = 0;
				++index;
				i += 2;
				continue;
			}
		}
		else
		{
			addToFileString += inputString[i];
			if (i + 1 < sizeOfString && inputString[i + 1] != ',')
			{
				continue;
			}
			else
			{
				unsigned size = addToFileString.length();
				file.write((const char*)& size, sizeof(unsigned));
				sizeOfRow += sizeof(unsigned);

				file.write(addToFileString.c_str(), size);
				sizeOfRow += size;
				addToFileString = "";
				++index;
				i += 2;
				continue;
			}
		}
	}
	file.close();
	inputInInfoFile(sizeOfRow);
}

void Table::inputInInfoFile(int sizeOfRow)
{
	std::string nameOfTableInfo = nameOfTable + ".info";
	std::ofstream fileWithInfo(nameOfTableInfo, std::ios::binary | std::ios::app);
	if (fileWithInfo.is_open())
	{
		fileWithInfo.write((const char*)&sizeOfRow, sizeof(int));

		fileWithInfo.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}
}

int Table::getSizeOfFileInfo()
{
	std::string nameOfTableInfo = nameOfTable + ".info";
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

void Table::generateInfoArray()
{
	std::string nameOfInfoFile = nameOfTable + ".info";
	std::ifstream fileInfo(nameOfInfoFile, std::ios::binary);
	int sizeOfFile = getSizeOfFileInfo();
	if (fileInfo.is_open())
	{
		for (int i = 0; i < sizeOfFile; ++i)
		{
			int addToInfoArray = 0;
			fileInfo.read((char*)&addToInfoArray, sizeof(int));
			arrayInfo.pushBack(addToInfoArray);
		}
		fileInfo.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}
}

bool Table::findElementToDelete() const
{
	unsigned sizeOfArray = arrayInfo.sizeOfArray();
	for (unsigned i = 1; i < sizeOfArray; ++i)
	{
		if (arrayInfo[i] <= 0)
		{
			return true;
		}
	}

	return false;
}

bool Table::isAllDeleted() const
{
	unsigned sizeOfArray = arrayInfo.sizeOfArray();
	for (unsigned i = 1; i < sizeOfArray; ++i)
	{
		if (arrayInfo[i] > 0)
		{
			return false;
		}
	}

	return true;
}

int Table::sumOfPreviousIndexInInfoFile(int index) const
{
	int sum = 0;
	for (int i = 0; i <= index; ++i)
	{
		sum += abs(arrayInfo[i]);
	}

	return sum;
}

int Table::movePointerInPosition(int index) const
{
	int sum = 0;
	for (int i = 0; i < index; ++i)
	{
		sum += abs(arrayInfo[i]);
	}

	return sum;
}

void Table::changeNumberOfColumn(int index)
{
	arrayInfo[index] *= (-1);
}

void Table::printRow(const RowArray& row)
{
	row.print();
}

void Table::printBy(const RowArray& row, const std::string& nameOfColumn)
{
	int index = 1;
	unsigned numberOfColumns = columnsInfo.getNumberOfColumns();
	for (unsigned i = 1; i < numberOfColumns; ++i)
	{
		if (columnsInfo[i].nameOfColumn == nameOfColumn)
		{
			index = i;
			break;
		}
	}

	row.printOnly(index);
}

void Table::createQueueForSelect(std::string& inputString, Queue<std::string>& queueForSelect)
{
	unsigned sizeOfString = inputString.length();
	char lastElement = inputString[sizeOfString - 1];
	std::string addToQueue;
	unsigned indexToRemove = 0;
	for (unsigned i = 0; i < sizeOfString; ++i)
	{
		if (inputString[i] == ' ')
		{
			if (addToQueue == "ORDER")
			{
				indexToRemove -= 5;
				break;
			}
			++indexToRemove;
			queueForSelect.pushBack(addToQueue);
			addToQueue = "";
		}
		else if (inputString[i] == '(') //|| inputString[i] == ')')
		{
			queueForSelect.pushBack("(");
			++indexToRemove;
			continue;
		}
		else if (inputString[i] == ')')
		{
			++indexToRemove;
			queueForSelect.pushBack(addToQueue);
			addToQueue = "";
			queueForSelect.pushBack(")");
			++i;
			continue;
		}
		else
		{
			addToQueue += inputString[i];
			++indexToRemove;
		}
	}
	if (addToQueue != "ORDER" && lastElement != ')')
	{
		queueForSelect.pushBack(addToQueue);
	}
	inputString.erase(0, indexToRemove);
}

bool Table::haveOrderBy(const std::string& inputString)
{
	unsigned sizeOfString = inputString.length();
	if (isdigit(inputString[sizeOfString - 1]) || inputString[sizeOfString - 1] == '\"')
	{
		return false; 
	}
	return true;
}

bool Table::haveFunctional(RowArray& row, Queue<std::string>& queue)
{
	Stack<std::string> resultStack;
	unsigned sizeOfQueue = queue.getSize();
	while (sizeOfQueue)
	{
		if (queue.front() == "(")
		{
			resultStack.push(queue.front());
			queue.pop();
			--sizeOfQueue;
			queue.pushBack(resultStack.top());
		}
		std::string column = queue.front();
		queue.pop();
		--sizeOfQueue;
		queue.pushBack(column);
		if (column == "And" || column == "Or")
		{
			resultStack.push(column);
			column = queue.front();
			queue.pop();
			--sizeOfQueue;
			queue.pushBack(column);
		}
		unsigned index = 1;
		unsigned numberOfColumn = columnsInfo.getNumberOfColumns();
		for (unsigned i = 1; i < numberOfColumn; ++i)
		{
			if (columnsInfo[i].nameOfColumn == column)
			{
				index = i;
				break;
			}
		}

		std::string operation = queue.front();
		queue.pop();
		--sizeOfQueue;
		queue.pushBack(operation);

		if (columnsInfo[index].typeOfColumn == Column::Type::INT)
		{
			std::string stringToConvert = queue.front();
			queue.pop();
			--sizeOfQueue;
			queue.pushBack(stringToConvert);
			int number = convertStringToInt(stringToConvert);
			row[index].isTrue(operation, &number) ? resultStack.push("True") : resultStack.push("False");
		}
		else
		{
			std::string stringToCheck = queue.front();
			queue.pop();
			--sizeOfQueue;
			const char* check = stringToCheck.c_str();
			queue.pushBack(stringToCheck);
			row[index].isTrue(operation, check) ? resultStack.push("True") : resultStack.push("False");
		}

		if (queue.front() == "And" || queue.front() == "Or")
		{
			resultStack.push(queue.front());
			queue.pop();
			--sizeOfQueue;
			queue.pushBack(resultStack.top());
		}

		if (queue.front() == ")")
		{
			std::string temp = queue.front();
			queue.pop();
			--sizeOfQueue;
			queue.pushBack(temp);

			bool second = resultStack.top() == "True";
			resultStack.pop();
			int operation = resultStack.top() == "And" ? 1 : 2;
			resultStack.pop();
			bool first = resultStack.top() == "True";
			resultStack.pop();
			resultStack.pop();//maham i (
			if (operation == 1)
			{
				(first && second) ? resultStack.push("True") : resultStack.push("False");
			}
			else
			{
				(first || second) ? resultStack.push("True") : resultStack.push("False");
			}
		}

	}

	bool second = resultStack.top() == "True";
	resultStack.pop();
	if (resultStack.isEmpty())
	{
		return second;
	}

	int operation = resultStack.top() == "And" ? 1 : 2;
	resultStack.pop();
	bool first = resultStack.top() == "True";
	resultStack.pop();
	if (operation == 1)
	{
		if (first && second)
		{
			return true;
		}
		return false;
	}
	else
	{
		if (first || second)
		{
			return true;
		}
		return false;
	}

}

int Table::convertStringToInt(std::string& stringToConvert)
{
	unsigned sizeOfString = stringToConvert.length();
	int returnInt = stringToConvert[0] - '0';
	for (unsigned i = 1; i < sizeOfString; ++i)
	{
		returnInt *= 10;
		returnInt += stringToConvert[i] - '0';
	}

	return returnInt;
}

void Table::inputTable(std::string& inputString, const std::string& nameOfColumn)
{
	generateInfoArray();
	if (isAllDeleted())
	{
		std::cout << "The table have not rows.\n";
		return;
	}

	unsigned printResultOfFunction = 0;

	Queue<std::string> queueForSelect;
	createQueueForSelect(inputString, queueForSelect);

	unsigned numberOfColumns = columnsInfo.getNumberOfColumns();

	std::string nameOfTableFile = nameOfTable + ".tbl";
	std::ifstream file(nameOfTableFile, std::ios::binary);
	if (file.is_open())
	{
		int movePointer = arrayInfo[0];
		file.seekg(movePointer, std::ios::beg);

		unsigned numberOfRowInInfoFile = arrayInfo.sizeOfArray();
		bool flag = true;
		for (unsigned i = 1; i < numberOfRowInInfoFile; ++i)
		{
			if (arrayInfo[i] <= 0)
			{
				file.seekg(sumOfPreviousIndexInInfoFile(i), std::ios::beg);
				continue;
			}
			RowArray row(numberOfColumns);
			for (unsigned i = 0; i < numberOfColumns; ++i)
			{
				if (columnsInfo[i].typeOfColumn == Column::Type::INT)
				{
					int typeInt = 0;
					file.read((char*)&typeInt, sizeof(int));
					IntType pushElement(typeInt);
					row.pushBack(pushElement);
				}
				else
				{
					unsigned sizeOfString = 0;
					char* buffer;
					file.read((char*)&sizeOfString, sizeof(unsigned));
					buffer = new char[sizeOfString + 1];
					file.read(buffer, sizeOfString);
					std::string typeString;
					typeString.append(buffer, sizeOfString);
					delete[] buffer;

					StringType pushElement(typeString);
					row.pushBack(pushElement);
				}
			}

			if (haveFunctional(row, queueForSelect))
			{
				++printResultOfFunction;
				if (nameOfColumn != "Remove")
				{
					if (nameOfColumn != "*")
					{
						if (flag)
						{
							columnsInfo.printOnly(nameOfColumn);
							flag = false;
						}
						printBy(row, nameOfColumn);
					}
					else
					{
						if (flag)
						{
							columnsInfo.print();
							flag = false;
						}
						printRow(row);
					}
				}
				else
				{
					changeNumberOfColumn(i);
				}
			}
		}

		file.close();
	}

	if (nameOfColumn == "Remove")
	{
		std::cout << printResultOfFunction << " rows removed.\n";
	}
	else
	{
		std::cout << "Total " << printResultOfFunction << " rows selected.\n";
	}
}

void Table::inputTableCaseOrderBy(std::string& inputString, const std::string& nameOfColumn)
{
	generateInfoArray();
	if (isAllDeleted())
	{
		std::cout << "The table have not rows.\n";
		return;
	}

	Queue<std::string> queueForSelect;
	createQueueForSelect(inputString, queueForSelect);

	unsigned numberOfColumns = columnsInfo.getNumberOfColumns();

	DynamicArray<int> indexArray;
	DynamicArray<std::string> arrayToSort;

	inputString.erase(0, 9);
	int indexOfColumn = 1;
	for (unsigned i = 1; i < numberOfColumns; ++i)
	{
		if (columnsInfo[i].nameOfColumn == inputString)
		{
			indexOfColumn = i;
			break;
		}
	}

	unsigned intOrString = 2;
	if (columnsInfo[indexOfColumn].typeOfColumn == Column::Type::INT)
	{
		intOrString = 1;
	}

	std::string nameOfTableFile = nameOfTable + ".tbl";
	std::ifstream file(nameOfTableFile, std::ios::binary);
	if (file.is_open())
	{
		file.seekg(arrayInfo[0], std::ios::beg);

		unsigned numberOfRowInInfoFile = arrayInfo.sizeOfArray();

		for (unsigned i = 1; i < numberOfRowInInfoFile; ++i)
		{
			if (arrayInfo[i] <= 0)
			{
				file.seekg(sumOfPreviousIndexInInfoFile(i), std::ios::beg);
				continue;
			}
			RowArray row(numberOfColumns);
			for (unsigned i = 0; i < numberOfColumns; ++i)
			{
				if (columnsInfo[i].typeOfColumn == Column::Type::INT)
				{
					int typeInt = 0;
					file.read((char*)&typeInt, sizeof(int));
					IntType pushElement(typeInt);
					row.pushBack(pushElement);
				}
				else
				{
					unsigned sizeOfString = 0;
					char* buffer;
					file.read((char*)&sizeOfString, sizeof(unsigned));
					buffer = new char[sizeOfString + 1];
					file.read(buffer, sizeOfString);
					std::string typeString;
					typeString.append(buffer, sizeOfString);
					delete[] buffer;

					StringType pushElement(typeString);
					row.pushBack(pushElement);
				}
			}

			if (haveFunctional(row, queueForSelect))
			{
				IntType* addToArray = dynamic_cast<IntType*>(&row[0]);
				int convert = addToArray->getData();

				indexArray.pushBack(convert);

				if (intOrString == 1)
				{
					IntType* addToArraySort = dynamic_cast<IntType*>(&row[indexOfColumn]);
					int getData = addToArraySort->getData();

					arrayToSort.pushBack(std::to_string(getData));
				}
				else
				{
					StringType* temp = dynamic_cast<StringType*>(&row[indexOfColumn]);
					std::string addToArray = temp->getData();

					arrayToSort.pushBack(addToArray);
				}
			}
		}

		file.close();
	}
	sortArray(indexArray, arrayToSort, intOrString);
	printSortRows(indexArray, nameOfColumn);
}

void Table::saveArrayInInfoFile() const
{
	std::string nameOfInfoFile = nameOfTable + ".info";
	std::ofstream file(nameOfInfoFile, std::ios::binary | std::ios::trunc);
	if (file.is_open())
	{
		int sizeOfArray = arrayInfo.sizeOfArray();
		for (int i = 0; i < sizeOfArray; ++i)
		{
			file.write((const char*)&arrayInfo[i], sizeof(int));
		}
	}
}

void Table::compactInfoFile()
{
	unsigned sizeOfInfoFile = arrayInfo.sizeOfArray();
	for (unsigned i = 0; i < sizeOfInfoFile; ++i)
	{
		if (arrayInfo[i] <= 0)
		{
			arrayInfo.removeElement(i);
		}
	}

	saveArrayInInfoFile();
}

void Table::sortArray(DynamicArray<int>& indexArray, DynamicArray<std::string>& arrayToSort, unsigned intOrString)
{
	unsigned sizeOfArray = arrayToSort.sizeOfArray();
	bool swapped;
	for (unsigned i = 0; i < sizeOfArray - 1; ++i)
	{
		swapped = false;
		for (unsigned j = 0; j < sizeOfArray - i - 1; ++j)
		{
			if (intOrString == 1)
			{
				int numberInj = convertStringToInt(arrayToSort[j]);
				int numberInj1 = convertStringToInt(arrayToSort[j + 1]);
				if (numberInj > numberInj1)
				{
					std::swap(arrayToSort[j], arrayToSort[j + 1]);
					std::swap(indexArray[j], indexArray[j + 1]);
					swapped = true;
				}
			}
			else
			{
				if (arrayToSort[j] > arrayToSort[j + 1])
				{
					std::swap(arrayToSort[j], arrayToSort[j + 1]);
					std::swap(indexArray[j], indexArray[j + 1]);
					swapped = true;
				}
			}
		}

		if (swapped == false)
		{
			break;
		}
	}
}

void Table::printSortRows(DynamicArray<int>& indexArray, const std::string& nameOfColumn)
{
	if (nameOfColumn == "*")
	{
		columnsInfo.print();
	}
	else
	{
		columnsInfo.printOnly(nameOfColumn);
	}

	std::string nameOfTableFile = nameOfTable + ".tbl";
	std::ifstream fileWithTable(nameOfTableFile, std::ios::binary);

	unsigned sizeOfArray = indexArray.sizeOfArray();
	unsigned numberOfColumns = columnsInfo.getNumberOfColumns();

	if (fileWithTable.is_open())
	{
		for (unsigned i = 0; i < sizeOfArray; ++i)
		{
			fileWithTable.seekg(movePointerInPosition(indexArray[i]), std::ios::beg);

			RowArray row(numberOfColumns);
			for (unsigned i = 0; i < numberOfColumns; ++i)
			{
				if (columnsInfo[i].typeOfColumn == Column::Type::INT)
				{
					int typeInt = 0;
					fileWithTable.read((char*)&typeInt, sizeof(int));
					IntType pushElement(typeInt);
					row.pushBack(pushElement);
				}
				else
				{
					unsigned sizeOfString = 0;
					char* buffer;
					fileWithTable.read((char*)&sizeOfString, sizeof(unsigned));
					buffer = new char[sizeOfString + 1];
					fileWithTable.read(buffer, sizeOfString);
					std::string typeString;
					typeString.append(buffer, sizeOfString);
					delete[] buffer;

					StringType pushElement(typeString);
					row.pushBack(pushElement);
				}
			}

			if (nameOfColumn == "*")
			{
				printRow(row);
			}
			else
			{
				printBy(row, nameOfColumn);
			}
		}

	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	std::cout << "Total " << sizeOfArray << " rows selected.\n";
}