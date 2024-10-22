#include "Util.hpp"
#include <sstream>



// FIXME: error handling or copy from cpp06
std::string	toString(const int value)
{
	std::ostringstream oss;
	oss << value;
	return (oss.str());
}

std::string toString(const size_t value)
{
	std::ostringstream oss;
	oss << value;
	return (oss.str());
}

std::string toString(const ssize_t value)
{
	std::ostringstream oss;
	oss << value;
	return (oss.str());
}

std::string toString(const std::vector<std::string>& values)
{
	std::string result = "";
	for (std::vector<std::string>::const_iterator it = values.begin(); it != values.end(); ++it)
		result += *it;
	return (result);
}

size_t		toSizeT(const std::string& value)
{
	std::istringstream	iss(value);
	size_t				result;
	iss >> result;
	return (result);
}