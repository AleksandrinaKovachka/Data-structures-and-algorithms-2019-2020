#include "pch.h"
#include "Tree.h"

Tree::~Tree()
{
	clear(root);
}

Tree::Tree(const Tree& r)
{
	root = copy(r.root);
}

size_t Tree::size() const
{
	return count(root);
}

void Tree::makeTree(std::string& inputString)
{
	this->root = makeTreeByStringInput(inputString);
}

TreeNode* Tree::makeTreeByStringInput(std::string& inputString)
{
	TreeNode *root = new TreeNode;
	const char *stringToTree = inputString.c_str() + 1;

	return helpMakeTree(stringToTree, root);
}

TreeNode* Tree::helpMakeTree(const char*& stringToTree, TreeNode* root)
{
	if (*stringToTree == '(')
	{
		++stringToTree;
		while (*stringToTree != ')')
		{
			if (*stringToTree == '[')
			{
				TreeNode *children = new TreeNode;
				++stringToTree;
				helpMakeTree(stringToTree, children);
				root->children.push_back(children);
			}
	
			++stringToTree;
		}
		++stringToTree;
	}
	return root;
}

void Tree::arrayInTree(int *arr)
{
	helpArrayInTree(root, arr);
}

void Tree::helpArrayInTree(TreeNode* &root, int*& arr)
{
	if (root == nullptr)
	{
		return;
	}

	root->data = *arr;
	++arr;
	for (std::list <TreeNode*>::iterator it = root->children.begin(); it != root->children.end(); it++)
	{
		helpArrayInTree(*it, arr);
	}
}

bool Tree::isGracefully()
{
	unsigned size = this->size() - 1;
	int* arr = new int[size];
	helpGracefully(root, arr);

	for (size_t i = 0; i < size - 1; i++)
	{
		for (size_t j = i + 1; j < size; j++)
		{
			if (arr[i] == arr[j])
			{
				delete[] arr;
				return false;
			}
		}
	}

	delete[] arr;
	return true;
}

void Tree::helpGracefully(TreeNode* root, int*& arr)
{
	if (root == nullptr)
	{
		return;
	}

	for (std::list <TreeNode*>::iterator it = root->children.begin(); it != root->children.end(); it++)
	{
		*arr = abs(root->data - (*it)->data);
		++arr;
		helpGracefully(*it, arr);
	}
}

void Tree::outputTreeString(std::string& outputString)
{
	pushTreeInString(this->root, outputString);
}

void Tree::pushTreeInString(TreeNode* &root, std::string& outputString)
{
	outputString += '[';
	outputString += std::to_string(root->data);
	if (!root->children.empty())
	{
		outputString += '(';
	}
	for (std::list<TreeNode*>::iterator it = root->children.begin(); it != root->children.end(); ++it)
	{
		pushTreeInString(*it, outputString);

	}
	if (!root->children.empty())
	{
		outputString += ')';
	}
	outputString += ']';
}

void Tree::clear(TreeNode* r)
{
	if (!r)
	{
		return;
	}

	for (std::list <TreeNode*> ::iterator it = r->children.begin(); it != r->children.end(); it++)
	{
		clear(*it);
	}
	delete r;
}

TreeNode* Tree::copy(TreeNode* r)
{
	if (!r)
	{
		return nullptr;
	}

	TreeNode* node = new TreeNode(r->data);

	for (std::list <TreeNode*> ::iterator it = r->children.begin(); it != r->children.end(); it++)
	{
		node->children.push_back(copy(*it));
	}

	return node;
}

size_t Tree::count(TreeNode* r) const
{
	if (!r)
	{
		return 0;
	}

	size_t cnt = 1;

	for (std::list <TreeNode*>::iterator it = r->children.begin(); it != r->children.end(); it++)
	{
		cnt += count(*it);
	}

	return cnt;
}
