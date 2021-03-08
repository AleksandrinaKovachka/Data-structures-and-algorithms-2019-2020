#pragma once
#include "pch.h"
#include <iostream>
#include "Stack.h"
#include <fstream>
#include <string>

const int BUFFER_SIZE = 256;

bool currectStringList(const std::string& stringList);
void returnList(const std::string& stringList, const char* nameOfUncompressedFile);
int ifHaveDigit(const std::string& stringList, int index, const char* nameOfUncompressedFile);
void repeatList(Stack<int>& stackForInt, const char* nameOfUncompressedFile);
int moreThanOneDigit(const std::string& stringList, int index, Stack<int>& stackForInt);
int ifHaveSlash(const std::string& stringList, int index, const char* nameOfUncompressedFile);
int ifHaveQuotes(const std::string& stringList, int index, const char* nameOfUncompressedFile);
void inputFromFile(const char* nameOfCompressedFile, const char* nameOfUnompressedFile);
void addCharToFile(const char *nameOfFile, const char& element);
int getSizeOfFile(const char *nameOfFile);
