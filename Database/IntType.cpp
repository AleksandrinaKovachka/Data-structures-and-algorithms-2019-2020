#include "pch.h"
#include "IntType.h"

IntType::IntType(int data)
{
	this->data = data;
}

std::string IntType::getType()
{
	return "int";
}

IntType* IntType::clone() const
{
	return new IntType(*this);
}

bool IntType::isTrue(std::string& operation, const void* value) const
{
	if (operation == "==")
	{
		return data == *reinterpret_cast<const int*>(value);
	}
	else if (operation == "!=")
	{
		return data != *reinterpret_cast<const int*>(value);
	}
	else if (operation == ">=")
	{
		return data >= *reinterpret_cast<const int*>(value);
	}
	else if (operation == ">")
	{
		return data > *reinterpret_cast<const int*>(value);
	}
	else if (operation == "<=")
	{
		return data <= *reinterpret_cast<const int*>(value);
	}
	else if (operation == "<")
	{
		return data < *reinterpret_cast<const int*>(value);
	}
}

int IntType::getData() const
{
	return data;
}

void IntType::print() const
{
	std::cout << '|' << std::setw(11) << data;
}