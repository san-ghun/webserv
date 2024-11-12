#include "Util.hpp"
#include <sstream>
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>


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

////////////////////////////////////////////////////////////////////////////////
/// Utility functions: File, Directory
////////////////////////////////////////////////////////////////////////////////

/// @brief Checks if a file exists.
/// @param path The path of the file.
bool	isFile(const std::string path)
{
	struct stat buffer;
	if (stat(path.c_str(), &buffer) != 0)
		return (false);
	if (S_ISREG(buffer.st_mode))
		return (true);
	return (false);
}

/// @brief Checks if a file exists.
/// @param path The path of the file.
bool	isDir(const std::string path)
{
	struct stat buffer;

	if (stat(path.c_str(), &buffer) != 0)
		return (false);
	if (S_ISDIR(buffer.st_mode))
		return (true);
	return (false);
}

bool	createFile(const std::string& path)
{
	std::ofstream	file(path.c_str());
	if (file.is_open())
	{
		file.close();
		return (true);
	}
	return (false);
}

bool	deleteFile(const std::string& path)
{
	if (remove(path.c_str()) == 0)
		return (true);
	return (false);
}

bool	createDir(const std::string& path)
{
	if (isDir(path))
		return (true);
	if (mkdir(path.c_str(), 0777) == 0)
		return (true);
	return (false);
}

bool	deleteDir(const std::string& path)
{
	if (rmdir(path.c_str()) == 0)
		return (true);
	return (false);
}

////////////////////////////////////////////////////////////////////////////////
/// Utility functions: Permissions
////////////////////////////////////////////////////////////////////////////////

bool	hasWritePermission(const std::string& path)
{
	if (access(path.c_str(), W_OK) == 0)
		return (true);
	return (false);
}

bool	hasReadPermission(const std::string& path)
{
	if (access(path.c_str(), R_OK) == 0)
		return (true);
	return (false);
}

bool	hasExecutePermission(const std::string& path)
{
	if (access(path.c_str(), X_OK) == 0)
		return (true);
	return (false);
}

bool	deleteFileOrDir(const std::string& path)
{
	if (remove(path.c_str()) == 0)
		return (true);
	return (false);
}

////////////////////////////////////////////////////////////////////////////////
/// MultipartParser
////////////////////////////////////////////////////////////////////////////////
//

std::string	MultipartParser::_extractBoundary(const std::string& headers)
{
	std::string::size_type	pos = headers.find("boundary=");
	std::string				boundary;
	if (pos == std::string::npos)
		return ("");
	boundary = headers.substr(pos + 9);
	boundary = boundary.substr(0, boundary.find("\r\n"));
	
	return (boundary);
}

std::vector<std::string>	MultipartParser::_splitByBoundary(const std::string& body, const std::string& boundary)
{
	std::vector<std::string>	parts;
	std::string::size_type		pos = 0;
	std::string::size_type		nextPos = 0;

	while ((nextPos = body.find(boundary, pos)) != std::string::npos)
	{
		parts.push_back(body.substr(pos, nextPos - pos));
		pos = nextPos + boundary.size();
	}
	if (pos < body.size())
		parts.push_back(body.substr(pos));
	return (parts);
}
