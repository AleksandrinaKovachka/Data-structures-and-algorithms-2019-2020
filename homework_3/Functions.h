#pragma once
#include "pch.h"
#include <iostream>
#include <fstream>
#include <algorithm> 
#include "Tree.h"

void inputFromFile(const char* nameOfCompressedFile);
void returnFileWithGracefullyTree(std::string& fileString);
bool checkPermutation(Tree& tree);
