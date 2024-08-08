#include "Context.hpp"

////////////////////////////////////////////////////////////////////////////////
/// Constructor & Destructor
////////////////////////////////////////////////////////////////////////////////

Context::Context(ServerConfig& config, HttpRequest& request)
	: _serverConfig(config), _request(request), _location(_findLocation(*this))
{
}

Context::Context(const Context& other)
	: _serverConfig(other._serverConfig), _request(other._request), _location(other._location)
{
	if (!this->isInitialized())
		throw std::invalid_argument("Context not initialized");
}

Context& Context::operator=(const Context& other)
{
	if (this != &other)
	{
		_serverConfig = other._serverConfig;
		_location = other._location;
		_request = other._request;
	}
	return (*this);
}

Context::~Context()
{
}


////////////////////////////////////////////////////////////////////////////////
/// Public Methods
////////////////////////////////////////////////////////////////////////////////

const ServerConfig&	Context::getServer() const
{
	return (_serverConfig);
}

const Location&	Context::getLocation() const
{
	return (_location);
}

const HttpRequest&	Context::getRequest() const
{
	return (_request);
}

void	Context::setRequest(HttpRequest& request)
{
	_request = request;
}

void	Context::setServer(ServerConfig& config)
{
	_serverConfig = config;
}

void	Context::setLocation(Location& location)
{
	_location = location;
}

bool	Context::isInitialized() const
{
	if (&_serverConfig != NULL && &_location != NULL && &_request != NULL)
		return (true);
	return (false);
}

////////////////////////////////////////////////////////////////////////////////
/// Private Methods
////////////////////////////////////////////////////////////////////////////////

Location&	Context::_findLocation(Context& context) const
{
	const std::string& uri = context.getRequest().getUri();

	std::map<std::string, Location*>	locations = _serverConfig.map_locationObjs;
	std::string	matchedLocation = getMatchedLocation(uri, locations);
	if (!matchedLocation.empty())
		return (*locations.at(matchedLocation));
	throw	std::invalid_argument("No match found for path: " + uri);
}


/// @brief Finds the longest location path that matches the request URI.
///
/// This function extracts the path from the URI and searches for the most specific location
/// that matches this path. It starts with the full path and progressively checks shorter
/// paths up to the root, using the provided locations map from the `Config` instance.
///
/// @param path The path extracted from the URI to match against the locations.
/// @param locations A map of location paths to `Location` objects from the `Config` instance.
/// @return The longest matching `location path`. If no match is found, returns an `empty string`.
std::string getMatchedLocation(std::string path, const std::map<std::string, Location*>& locations)
{
	std::string	matched;
	std::string parentPath;

	if (path.empty())
		throw std::invalid_argument("Path cannot be empty");
	if (path == "/")
	{
		std::map<std::string, Location*>::const_iterator it = locations.find(path);
		if (it != locations.end())
			return (path);
		throw std::invalid_argument("No match found for path: " + path);
	}
	ssize_t lastSlashPos = path.find_last_of('/');
	path = path.substr(0, lastSlashPos + 1);
	while (!path.empty())
	{
		std::map<std::string, Location*>::const_iterator it = locations.find(path);
		if (it != locations.end())
		{
			matched = path;
			break;
		}
		path = getParentPath(path);
		if (path == "/")
			break;
	}
	return (matched);
}

std::string	getParentPath(const std::string& path)
{

	ssize_t	lastSlashPos = path.find_last_of('/');
	if (lastSlashPos == 0)
		return ("/");
	return (path.substr(0, lastSlashPos));
}