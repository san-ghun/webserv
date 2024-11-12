#ifndef UTIL_HPP
# define UTIL_HPP

# include <string>
# include <vector>
# include <sstream>
# include <map>
# include <sys/types.h> 

std::string		toString(const int value);
std::string		toString(const size_t value);
std::string		toString(const ssize_t value);
std::string		toString(const std::vector<std::string>& values);

size_t			toSizeT(const std::string& value);


bool	isDir(const std::string path);
bool	createDir(const std::string& path);
bool	deleteDir(const std::string& path);

bool	isFile(const std::string path);
bool	createFile(const std::string& path);
bool	deleteFile(const std::string& path);

bool	hasWritePermission(const std::string& path);
bool	hasReadPermission(const std::string& path);
bool	hasExecutePermission(const std::string& path);
bool	deleteFileOrDir(const std::string& path);

class MultipartParser
{


private:

	static std::string              _extractBoundary(const std::string& headers);
	static std::vector<std::string>	_splitByBoundary(const std::string& body, const std::string& boundary);


private:
	MultipartParser();
	~MultipartParser();
};

#endif