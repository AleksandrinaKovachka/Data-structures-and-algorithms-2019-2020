#include "pch.h"
#include <iostream>
#include "Database.h"

int main()
{
	Database database;
	std::string inputString;
	do
	{
		std::cout << "FMISql> ";
		getline(std::cin, inputString);

		if (inputString == "Quit")
		{
			std::cout << "Goodbye, master ;(\n";
			break;
		}
		else
		{
			database.selectFunction(inputString);
		}
	} while (true);

	return 0;
}

