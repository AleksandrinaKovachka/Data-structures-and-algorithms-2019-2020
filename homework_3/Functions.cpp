#include "pch.h"
#include "Functions.h"

void inputFromFile(const char* nameFileWithTree)
{
	std::string line;
	std::ifstream file(nameFileWithTree);
	if (file.is_open())
	{
		while (std::getline(file, line))
		{
			returnFileWithGracefullyTree(line);
		}
	}
	else
	{
		std::cout << "Unable to open\n";
	}

	file.close();
}

void returnFileWithGracefullyTree(std::string& fileString)
{
	Tree tree;
	tree.makeTree(fileString);

	if (checkPermutation(tree))
	{
		std::string outputString;
		tree.outputTreeString(outputString);
		std::ofstream file("FileWithGracefullyTree");
		if (file.is_open())
		{
			file << outputString;
		}
		else
		{
			std::cout << "Unable to open\n";
		}

		file.close();
	}
	else
	{
		std::cout << "The tree can not be gracefully\n";
	}
}

bool checkPermutation(Tree& tree)
{
	int size = tree.size();
	int *arr = new int[size];
	int next = 1;

	for (int i = 0; i < size; ++i)
	{
		arr[i] = next;
		next += 2;
	}

	do {
		tree.arrayInTree(arr);
		if (tree.isGracefully())
		{
			delete[] arr;
			return true;
		}
	} while (std::next_permutation(arr, arr + size));

	return false;
	delete[] arr;
}