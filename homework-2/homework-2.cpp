#include "pch.h"
#include <iostream>
#include "Functions.h"

int main(int argc, char** argv)
{
	char* compressedFileName = argv[1];
	char* uncompressedFileName = argv[2];

	inputFromFile(compressedFileName, uncompressedFileName);

	return 0;
}

