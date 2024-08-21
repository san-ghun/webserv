#include "Util.hpp"
#include <sstream> // Include the header file for std::ostringstream

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

void	removeCRLF(std::string* line)
{
	size_t	segmentSize = line->size();
	if (segmentSize >= 2)
	{
		if ((*line)[segmentSize - 2] == '\r' && (*line)[segmentSize - 1] == '\n')
			line->erase(segmentSize - 2);
	}
}

std::string removeCRLF(const std::string& line)
{
	std::string result = line;
	size_t	segmentSize = result.size();
	if (segmentSize >= 2)
	{
		if (result[segmentSize - 2] == '\r' && result[segmentSize - 1] == '\n')
			result.erase(segmentSize - 2);
		return (result);
	}
	return ("");
}
