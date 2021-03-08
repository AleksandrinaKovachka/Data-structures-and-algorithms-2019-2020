#include "pch.h"
#include "Functions.h"

//Test if string is not correct
bool currectStringList(const std::string& stringList)
{
	int lengthOfString = stringList.length();
	int countForQuotes = 0;
	Stack<char> stackForCheck;
	if (stringList[lengthOfString - 1] == '\\')
	{
		std::cout << "The last symbol can not be \\" << std::endl;
		return false;
	}
	for (int i = 0; i < lengthOfString; ++i)
	{
		if (stringList[i] == '"')
		{
			++countForQuotes;
			if ((i - 1) >= 0 && stringList[i - 1] == '//')
			{
				--countForQuotes;
			}
			continue;
		}
		else if (stringList[i] == 0)
		{
			if (stringList[i + 1] == '(')
			{
				std::cout << "You can not want to uncompress 0\n";
				return false;
			}
		}
		else if (stringList[i] == '(')
		{
			if (stringList[i - 1] > '1' && stringList[i - 1] < '9')
			{
				stackForCheck.push(stringList[i]);
			}
			else
			{
				std::cout << "Before brackets do not have number\n";
				return false;
			}
		}
		else if (stringList[i] == ')')
		{
			if (stackForCheck.top() != '(')
			{
				std::cout << "The brackets is not correct\n";
				return false;
			}
			else
			{
				stackForCheck.pop();
				continue;
			}
		}
	}

	if (countForQuotes % 2 != 0)
	{
		std::cout << "The quotes is not correct\n";
		return false;
	}

	if (!stackForCheck.empty())
	{
		std::cout << "The brackets is not correct\n";
		return false;
	}

	return true;
}

//------------------------------------------------------------------------------------

void returnList(const std::string& stringList, const char* nameOfUncompressedFile)
{
	if (currectStringList(stringList))
	{
		int lengthOfString = stringList.length();
		for (int i = 0; i < lengthOfString; ++i)
		{
			if (stringList[i] >= '1' && stringList[i] <= '9')
			{
				i = ifHaveDigit(stringList, i, nameOfUncompressedFile);
				continue;
			}
			else if (stringList[i] == '\\')
			{
				i = ifHaveSlash(stringList, i, nameOfUncompressedFile);
				continue;
			}
			else if (stringList[i] == '"')
			{
				i = ifHaveQuotes(stringList, i, nameOfUncompressedFile);
				continue;
			}
			else
			{
				addCharToFile(nameOfUncompressedFile, stringList[i]);
			}
		}
	}
}

int ifHaveDigit(const std::string& stringList, int index, const char* nameOfUncompressedFile)
{
	Stack<int> stackForInt;
	//Stack<char> stackForList;
	int lengthOfString = stringList.length();
	int i = index;
	for (i; i < lengthOfString; ++i)
	{
		if (stringList[i] >= '1' && stringList[i] <= '9')
		{
			stackForInt.push(stringList[i] - '0');
			i = moreThanOneDigit(stringList, i, stackForInt);
			if (stringList[i] == '(')
			{
				int sizeOfFile = getSizeOfFile(nameOfUncompressedFile);
				stackForInt.push(sizeOfFile);
				continue;
			}
			else
			{
				std::cout << "Wrong input\n";
				stackForInt.pop();
				break;
			}
		}
		else
		{
			if (stringList[i] != ')')
			{
				if (stringList[i] == '\\')
				{
					i = ifHaveSlash(stringList, i, nameOfUncompressedFile);
					continue;
				}
				else if (stringList[i] == '"')
				{
					i = ifHaveQuotes(stringList, i, nameOfUncompressedFile);
					continue;
				}
				else
				{
					addCharToFile(nameOfUncompressedFile, stringList[i]);
				}
			}
			else
			{
				repeatList(stackForInt, nameOfUncompressedFile);
			}

		}
	}

	return i;
}

void repeatList(Stack<int>& stackForInt, const char* nameOfUncompressedFile)
{
	char buffer[BUFFER_SIZE];
	int indexToWrite = stackForInt.top();
	stackForInt.pop();
	int timesToWrite = stackForInt.top() - 1;
	stackForInt.pop();
	int howElementsToWrite = getSizeOfFile(nameOfUncompressedFile) - indexToWrite;
	if (BUFFER_SIZE > howElementsToWrite)
	{
		std::ifstream fileWithUncompressed(nameOfUncompressedFile);
		if (fileWithUncompressed.is_open())
		{
			fileWithUncompressed.seekg(indexToWrite, std::ios::beg);
			fileWithUncompressed.read(buffer, howElementsToWrite);
		}
		else
		{
			std::cout << "Unable to open\n";
		}
		fileWithUncompressed.close();

		std::ofstream writeToFile(nameOfUncompressedFile, std::ios::app);
		if (writeToFile.is_open())
		{
			while (timesToWrite > 0)
			{
				writeToFile.write(buffer, howElementsToWrite);
				--timesToWrite;
			}
		}
		else
		{
			std::cout << "Unable to open\n";
		}
		writeToFile.close();
	}
	else
	{
		int timesToWriteElements = timesToWrite * howElementsToWrite;
		while (timesToWriteElements > BUFFER_SIZE)
		{
			std::ifstream fileWithUncompressed(nameOfUncompressedFile);
			if (fileWithUncompressed.is_open())
			{
				fileWithUncompressed.seekg(indexToWrite, std::ios::beg);
				fileWithUncompressed.read(buffer, BUFFER_SIZE);
				indexToWrite += BUFFER_SIZE;
				timesToWriteElements -= BUFFER_SIZE;
			}
			else
			{
				std::cout << "Unable to open\n";
			}
			fileWithUncompressed.close();

			std::ofstream writeToFile(nameOfUncompressedFile, std::ios::app);
			if (writeToFile.is_open())
			{
				writeToFile.write(buffer, BUFFER_SIZE);
			}
			else
			{
				std::cout << "Unable to open\n";
			}
			writeToFile.close();
		}

		if (timesToWriteElements != 0)
		{
			std::ifstream fileWithUncompressed(nameOfUncompressedFile);
			if (fileWithUncompressed.is_open())
			{
				fileWithUncompressed.seekg(indexToWrite, std::ios::beg);
				fileWithUncompressed.read(buffer, timesToWriteElements);
			}
			else
			{
				std::cout << "Unable to open\n";
			}
			fileWithUncompressed.close();

			std::ofstream writeToFile(nameOfUncompressedFile, std::ios::app);
			if (writeToFile.is_open())
			{
				writeToFile.write(buffer, timesToWriteElements);
			}
			else
			{
				std::cout << "Unable to open\n";
			}
			writeToFile.close();
		}
	}

}

int moreThanOneDigit(const std::string& stringList, int index, Stack<int>& stackForInt)
{
	int lengthOfString = stringList.length();
	for (int i = index + 1; i < lengthOfString; ++i)
	{
		if (stringList[i] >= '1' && stringList[i] <= '9')
		{
			int tempNumber = stackForInt.top() * 10 + stringList[i] - '0';
			stackForInt.pop();
			stackForInt.push(tempNumber);
		}
		else if (stringList[i] == '0')
		{
			int tempNumber = stackForInt.top() * 10;
			stackForInt.pop();
			stackForInt.push(tempNumber);
		}
		else
		{
			return i;
		}
	}
}

int ifHaveSlash(const std::string& stringList, int index, const char* nameOfUncompressedFile)
{
	addCharToFile(nameOfUncompressedFile, stringList[++index]);
	return index;
}

int ifHaveQuotes(const std::string& stringList, int index, const char* nameOfUncompressedFile)
{
	int lengthOfString = stringList.length();
	int i = index + 1;
	for (i; stringList[i] != '"' || i < lengthOfString; ++i)
	{
		if (stringList[i] == '\"')
		{
			addCharToFile(nameOfUncompressedFile, stringList[i]);
		}
		else
		{
			addCharToFile(nameOfUncompressedFile, stringList[i]);
		}
	}
	return i;
}

void inputFromFile(const char* nameOfCompressedFile, const char* nameOfUnompressedFile)
{
	std::string line;
	std::ifstream fileWithCompressed(nameOfCompressedFile);
	if (fileWithCompressed.is_open())
	{
		while (std::getline(fileWithCompressed, line))
		{
			returnList(line, nameOfUnompressedFile);
		}
	}
	else
	{
		std::cout << "Unable to open\n";
	}

	fileWithCompressed.close();
}

void addCharToFile(const char *nameOfFile, const char& element)
{
	std::ofstream fileWithUncompressed(nameOfFile, std::ios::app);
	if (fileWithUncompressed.is_open())
	{
		fileWithUncompressed << element;
	}
	else
	{
		std::cout << "Unable to open\n";
	}

	fileWithUncompressed.close();
}

int getSizeOfFile(const char *nameOfFile)
{
	std::ifstream inputSize(nameOfFile);
	int begin = inputSize.tellg();
	inputSize.seekg(0, std::ios::end);
	int end = inputSize.tellg();
	inputSize.close();

	return (end - begin);
}