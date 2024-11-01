#ifndef TEST_HPP
#define TEST_HPP

#include <map>
#include <string>
#include <stdexcept>
#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <map>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <fstream>
#include <sys/wait.h>
#include <fcntl.h>
#include <cstring>

class AMessage
{
	protected:
		std::string							_protocol_version;
		std::string							_first_line;
		std::string							_body;
		std::map<std::string, std::string>	_headers;

	public:
		AMessage();
		virtual ~AMessage();

		const std::string &getBody() const;
		const std::string &getVersion() const;
		const std::string &getHeaderOrDefault(const std::string &key, const std::string &default_value);
};

class Request : public AMessage
{
	private:
		std::string							_method;
		std::string							_uri;

	public:
		Request();

		const std::string &getMethod() const;
		const std::string &getUri() const;
		const std::string buildFirstLine() const;
		const std::string buildQueryString() const;
};

class CGIExecutor {
public:
    CGIExecutor();
    ~CGIExecutor();

    bool executeCGI(Request & request);
	bool isCGI(const std::string &path);

private:
    char** createCGIEnv(Request& request);
};

bool isCGI(const std::string &path);

#endif