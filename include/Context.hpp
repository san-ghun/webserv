#ifndef CONTEXT_H
#define CONTEXT_H

#include "Config.hpp"
#include "Location.hpp"
#include "HttpRequest.hpp"

std::string getMatchedLocation(std::string path, const std::map<std::string, Location*>& locations);
std::string	getParentPath(const std::string& path);

class Context {
public:
    Context(ServerConfig& config, HttpRequest& request);
    Context(const Context& other);
    ~Context();
    Context& operator=(const Context& other);

    const ServerConfig&	getServer() const;
    const Location&		getLocation() const;
	const HttpRequest&	getRequest() const;

	void setRequest(HttpRequest& request);
	void setServer(ServerConfig& config);
    void setLocation(Location& location);

    bool isInitialized() const;

private:
    ServerConfig&	_serverConfig;
	HttpRequest&	_request;
    Location&		_location;

    Location&			_findLocation(Context& context) const;
};

#endif // CONTEXT_H