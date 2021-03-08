#include "pch.h"
#include "ColumnsInfo.h"

ColumnsInfo::ColumnsInfo()
{
	numberOfColumns = 0;
	maxColumns = 0;
}

ColumnsInfo::ColumnsInfo(const ColumnsInfo& rhs)
{
	numberOfColumns = rhs.numberOfColumns;
	maxColumns = rhs.maxColumns;
	columns = new Column[maxColumns];
	for (unsigned i = 0; i < numberOfColumns; ++i)
	{
		columns[i] = rhs.columns[i];
	}
}

ColumnsInfo& ColumnsInfo::operator=(const ColumnsInfo& rhs)
{
	if (this != &rhs)
	{
		delete[] columns;

		numberOfColumns = rhs.numberOfColumns;
		maxColumns = rhs.maxColumns;
		columns = new Column[maxColumns];
		for (unsigned i = 0; i < numberOfColumns; ++i)
		{
			columns[i] = rhs.columns[i];
		}
	}

	return *this;
}

ColumnsInfo::~ColumnsInfo()
{
	delete[] columns;
}

void ColumnsInfo::setMaxColumns(unsigned max)
{
	maxColumns = max;
	columns = new Column[max];
}

unsigned ColumnsInfo::getNumberOfColumns() const
{
	return numberOfColumns;
}

void ColumnsInfo::pushColumn(const Column& column)
{
	if (numberOfColumns < maxColumns)
	{
		columns[numberOfColumns++] = column;
	}
}

Column& ColumnsInfo::operator[](unsigned index)
{
		return this->columns[index];
}

const Column& ColumnsInfo::operator[](unsigned index) const
{
	return this->columns[index];
}

void ColumnsInfo::createColumnsInfo(std::string& stringFromFile)
{
	unsigned sizeOfString = stringFromFile.length();
	std::string addToColumn;
	Column columnToAdd;
	columnToAdd.nameOfColumn = "index";
	columnToAdd.typeOfColumn = Column::Type::INT;
	pushColumn(columnToAdd);
	for (unsigned i = 1; i < sizeOfString; ++i)
	{
		if (stringFromFile[i] == ':')
		{
			columnToAdd.nameOfColumn = addToColumn;
			addToColumn = "";
			continue;
		}
		else if (stringFromFile[i] == ',' || stringFromFile[i] == ')')
		{
			if (addToColumn == "Int")
			{
				columnToAdd.typeOfColumn = Column::Type::INT;
			}
			else
			{
				columnToAdd.typeOfColumn = Column::Type::STRING;
			}
			pushColumn(columnToAdd);
			addToColumn = "";
			++i;
			continue;
		}
		else
		{
			addToColumn += stringFromFile[i];
		}
	}
}

void ColumnsInfo::print() const
{
	unsigned elementToShow = 0;
	for (unsigned i = 1; i < numberOfColumns; ++i)
	{
		if (columns[i].typeOfColumn == Column::Type::INT)
		{
			if (columns[i].nameOfColumn.length() > 11)
			{
				std::string outputString;
				outputString.append(columns[i].nameOfColumn, 7);
				outputString += "...";
				std::cout << '|' << std::setw(11) << outputString;
				elementToShow += 12;
			}
			else
			{
				std::cout << '|' << std::setw(11) << columns[i].nameOfColumn;
				elementToShow += 12;
			}
		}
		else
		{
			if (columns[i].nameOfColumn.length() > 20)
			{
				std::string outputString;
				outputString.append(columns[i].nameOfColumn, 16);
				outputString += "...";
				std::cout << '|' << std::setw(20) << outputString;
				elementToShow += 21;
			}
			else
			{
				std::cout << '|' << std::setw(20) << columns[i].nameOfColumn;
				elementToShow += 21;
			}
		}
	}
	std::cout << '|' << '\n';
	++elementToShow;
	for (unsigned i = 0; i < elementToShow; ++i)
	{
		std::cout << '-';
	}
	std::cout << '\n';
}

void ColumnsInfo::printOnly(const std::string& nameOfColumn) const
{
	unsigned elementToShow = 0;
	for (unsigned i = 1; i < numberOfColumns; ++i)
	{
		if (columns[i].nameOfColumn == nameOfColumn)
		{
			if (columns[i].typeOfColumn == Column::Type::INT)
			{
				if (columns[i].nameOfColumn.length() > 11)
				{
					std::string outputString;
					outputString.append(columns[i].nameOfColumn, 7);
					outputString += "...";
					std::cout << '|' << std::setw(11) << outputString;
					elementToShow += 12;
					break;
				}
				else
				{
					std::cout << '|' << std::setw(11) << columns[i].nameOfColumn << "|\n";
					elementToShow += 12;
					break;
				}
			}
			else
			{
				if (columns[i].nameOfColumn.length() > 20)
				{
					std::string outputString;
					outputString.append(columns[i].nameOfColumn, 16);
					outputString += "...";
					std::cout << '|' << std::setw(20) << outputString << "|\n";
					elementToShow += 21;
					break;
				}
				else
				{
					std::cout << '|' << std::setw(20) << columns[i].nameOfColumn << "|\n";
					elementToShow += 21;
					break;
				}
			}
		}
	}

	++elementToShow;
	for (unsigned i = 0; i < elementToShow; ++i)
	{
		std::cout << '-';
	}
	std::cout << '\n';
}