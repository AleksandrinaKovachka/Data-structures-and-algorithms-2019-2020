#pragma once
#include "pch.h"
#include <iostream>
#include <list>
#include <iterator>
#include <cassert>
#include <cmath>
#include <string>

struct TreeNode
{
	int data;
	std::list <TreeNode*> children;

	TreeNode(int data = 0)
	{
		this->data = data;
	}
};

class Tree
{
private:
	TreeNode* root;

public:
	Tree() : root(nullptr) {}
	~Tree();
	Tree(const Tree& r);

	size_t size() const;

	void makeTree(std::string& inputString); // 1

	void arrayInTree(int* arr); //2

	bool isGracefully(); //3

	void outputTreeString(std::string& outputString); //4


private:
	void clear(TreeNode* r);

	TreeNode* copy(TreeNode* r);

	size_t count(TreeNode* r) const;

	TreeNode* makeTreeByStringInput(std::string& inputString); //1

	TreeNode* helpMakeTree(const char*& stringToTree, TreeNode* root); //1

	void helpArrayInTree(TreeNode* &root, int*& arr); //2

	void helpGracefully(TreeNode* root, int*& arr); //3

	void pushTreeInString(TreeNode* &root, std::string& outputString); //4
};
