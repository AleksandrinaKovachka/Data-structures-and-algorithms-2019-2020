/*Избрах текстов файл за съхранение на входната структура дърво.
За описание на структурата избрах със символ [ да отбелязвам началото на корен - съответно с ] край,
със символи () да отбелязвам децата на корен. */

#include "pch.h"
#include "Functions.h"

int main(int argc, char** argv)
{
	char* nameFileWithTree = argv[1];

	inputFromFile(nameFileWithTree);

	return 0;
}