#include "test.hpp"

AMessage::AMessage()
	: _protocol_version("HTTP/1.1")
{}

AMessage::~AMessage() {}

const std::string &AMessage::getBody() const
{
	return _body;
}

const std::string &AMessage::getVersion() const
{
	return _protocol_version;
}

/*
    [ Goal ]
    헤더 존재 확인

    [ Param ]
    (1): key            // 찾고자 하는 요청 헤더의 이름
    (2): default_value  // key에 해당하는 헤더가 없을 경우 사용할 기본값

    [ Role ]
    사용자가 요청한 헤더(Param(1))가 _headers 맵에 있는지 확인하고,
    만약 없다면 기본값(Param(2))을 사용하여 _headers에 저장한 후 해당 값을 반환하는 함수

    [ Step ]
    1. Param(1): key가 _headers맵에 있는지 확인.
    2. _headers에 해당 키가 없거나 값이 비어 있다면
        Param(2): default_value을 사용하여 _headers에 저장.
    3. Param(1): key가 _headers맵에 있으면 해당 key값 리턴.
*/
const std::string &AMessage::getHeaderOrDefault(const std::string &key, const std::string &default_value)
{
	if (_headers[key].empty())
		_headers[key] = default_value;
	return _headers[key];
}

Request::Request()
	: _method("GET"), _uri("/")
{}

const std::string &Request::getMethod() const
{
	return (_method);
}

const std::string &Request::getUri() const
{
	return (_uri);
}

const std::string Request::buildFirstLine() const
{
	return (_method + " " + _uri + " " + _protocol_version);
}

const std::string Request::buildQueryString() const
{
	size_t pos = _uri.find('?');
	if (pos != std::string::npos)
	{
		return _uri.substr(pos);
	}
	return ("");
}

bool isCGI(const std::string & path)
{
	return path.find("/cgi-bin/") == 0 || path.find(".cgi") != std::string::npos
		|| path.find(".py") != std::string::npos;
}

/*  [F]
    [ Role ]
    CGI 프로그램을 실행하기 위해 필요한 환경 변수를 생성하는 역할

    [ Why ]
    executeCGI함수에서 CGI 프로그램에 해당하는 "execve"함수가 동작하기 위해선
    execve함수의 세번째 Param에 해당하는 "env"환경변수가 필요하다.
    설정해주지 않으면 함수가 실행되지 않는다.

    [ Process ]
    1. "env_map"이라는 맵 생성
    2. CGI 환경 변수들을 "env_map"에 저장
        REQUEST_METHOD  : 요청 메서드                (ex) "GET", "POST"
        SCRIPT_NAME     : 실행할 스크립트 경로 (URI)   (ex) "/upload_script.py"
        QUERY_STRING    : URI에서 전달되는 쿼리 문자열  (ex) "?query=apple"
        PROTOCOL        : 서버가 사용중인 프로토콜 버전  (ex) "HTTP/1.1"
        CONTENT_TYPE    : 클라이언트가 보낸 데이터의 형식 (request의 헤더부분에 나와있음)
        CONTENT_LENGTH  : 클라이언트가 보낸 데이터의 크기 (request의 헤더부분에 나와있음)
    
    [ Backgroun Knowledge ]
    1. HTTP Request standard form (based on Chrome)
        <php>
            <HTTP_METHOD>       <URI>                 <HTTP_VERSION>
            요청하는 동작          요청하는 리소스 경로      프로토콜의 버전

        ex) POST                /upload_script.py     HTTP/1.1
            (1)                 (2)                   (3)
        
        (1) POST매소드(동작)를 실행 할 때
        (3) HTTP/1.1 프로토콜을 기준으로
        (2) /upload_script.py 경로에 있는 스크립트를 찾아서 실행해라.

    2. QUERY_STRING
        웹 브라우저가 서버에 요청을 보낼 때 URL 뒤에 추가하는(디테일한) 정보
        ex) https://example.com/search?query=apple&limit=10
                                      ---------------------
                                      '?' 뒤에 오는 부분을 QUERY_STRING이라고 한다.

        Q: QUERY_STRING을 왜 별도로 처리해줘야 하는가
        A:'?'를 포함한 요청이 들어올 경우를 처리해주기 위해

    3. CONTENT_TYPE & CONTENT_LENGTH
        Chrome웹 브라우저는 HTTP 요청을 보낼 때 헤더 부분에
        "Content-Type" & "Content-Length"를 포함하는것이 일반적이다.
        = 크롬에서 정한 약속이다.
*/
char** CGIExecutor::createCGIEnv(Request& request)
{
    // CGI프로그램의 환경변수들을 저장할 맵 생성
    std::map<std::string, std::string> env_map;

    // CGI프로그램 작동에 필요한 환경변수들 모두 저장
    // 필요한 모든 소스들은 Request헤더에 이미 다 나와있음
    env_map["REQUEST_METHOD"] = request.getMethod().substr(0);
    env_map["SCRIPT_NAME"] = request.getUri();
    env_map["QUERY_STRING"] = request.buildQueryString();
    env_map["PROTOCOL"] = request.getVersion();
    env_map["CONTENT_TYPE"] = request.getHeaderOrDefault("Content-Type", "");
    env_map["CONTENT_LENGTH"] = request.getHeaderOrDefault("Content-Length", "0");

    char** env = new char*[env_map.size() + 1];
    int i = 0;
    for (std::map<std::string, std::string>::iterator it = env_map.begin(); it != env_map.end(); ++it)
	{
        std::string var = it->first + "=" + it->second;
        env[i] = new char[var.size() + 1];
        std::strcpy(env[i], var.c_str());
        i++;
    }
    env[i] = NULL;
    return env;
}
