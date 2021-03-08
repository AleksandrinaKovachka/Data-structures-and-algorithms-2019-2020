#include "pch.h"
#include "StringType.h"

StringType::StringType(std::string stringData)
{
	this->stringData = stringData;
}

std::string StringType::getType()
{
	return "string";
}

StringType* StringType::clone() const
{
	return new StringType(*this);
}

bool StringType::isTrue(std::string& operation, const void* value) const
{
	const char *str = reinterpret_cast<const char*>(value);
	if (operation == "==")
	{
		return stringData == std::string(str);
	}
	else if (operation == "!=")
	{
		return stringData != std::string(str);
	}
	else if (operation == ">=")
	{
		return stringData >= std::string(str);
	}
	else if (operation == ">")
	{
		return stringData > std::string(str);
	}
	else if (operation == "<=")
	{
		return stringData <= std::string(str);
	}
	else if (operation == "<")
	{
		return stringData < std::string(str);
	}
}

const std::string& StringType::getData() const
{
	return stringData;
}

void StringType::print() const
{
	std::cout << '|' << std::setw(20) << stringData;
}