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

/*  [ Finish ]
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

Response::Response() {}

Response::~Response() {}

/*  [ Finish ]
    [ Goal & Role ]
    Request로부터 필요한 정보를 받아 환경 변수를 생성하고
    자식 프로세스를 통해 CGI 프로그램을 실행하며
    실행 결과를 Response객체에 담아 반환하는 구조

    [ Background knowledge ]
    (1) Dynamic contents
        웹 서버는 아래 함수를 통해 클라이언트가 request한
        (CGI에 맞게 생성된)동적 콘텐츠를 생성할 수 있다.
        = 동적 컨텐츠는 사용자 요청에 따라 동적으로 변화할 수 있다.

        EX) 클릭 요청이 들어올 때마다 숫자가 올라가는 효과

    (2) const사용
        const char *path = request.getUri().c_str();
        = 반환된 문자열이 수정되지 않도록 하기 위해.

    (3) getUri().c_str();
        const char *path = request.getUri().c_str();
        C스타일 문자열(char*)은 std::string은 C++의 클래스 자료형을 받을 수 없다.
        = execve함수는 형 함수이기에 C언어에 맞게 데이터 타입을 맞춰줘야 한다.

    (4) 부모 프로세스가 자식에게 데이터를 넘기기 전에 자식 프로세스가 먼저 실행되는 경우
        자식 프로세스는 STDIN_FILENO를 통해 읽기를 시도하지만,
        데이터가 아직 부모 프로세스에 의해 작성되지 않았기 때문에 읽을 것이 없다.
        부모가 데이터를 보내기 전에 자식이 먼저 실행되면 자식은 데이터를 읽지 못하고 대기한다.
        = 파이프는 데이터가 없을 경우 블록킹이 발생하도록 설계되어 있다.

    [ Process ]
    (1) 환경 변수 설정 for [f] "execve" for CGI
        요청 기반 CGI프로그램이 필요한 환경 변수 설정 & 프린트 (제대로 셋팅 되었는지 체크)
        Setting envs for the function "execve" for CGI program.
        Print env to check if the environment settings are coorect.

    (2) 경로, 인수 설정 for [f] "execve" for CGI
        execve함수 실행에 필요한 경로와 인수 설정.
        = Setting the path and arguments for the CGI-Program.

        char *argv[1];
        문자열을 가리키는 포인터.
        크기가 1이기에 한개의 문자열만 가리킬 수 있다.

        Initialize string pointer to the NULL.
        = It points nothing yet.

    (3) 파이프 생성
        CGI프로그램의 표준 출력과 표준 입력을 처리하기 위해 두 개의 파이프 생성.

        1. parent_to_child_in
            부모 → 자식으로 (입력)을 전달하는 통로

        parent_to_child_in[1]: write
            |
            |       [1]
            |       ------------------------
            ----->  |  parent_to_child_in  |  -----> 
                    ------------------------       |      
                                         [0]       |
                                                   |
                                        parent_to_child_in[0]: read

        2. child_to_parent_out
            자식 → 부모로 (출력)을 전달하는 통로

        child_to_parent_out[1]: write
            |
            |       [1]
            |       -------------------------
            ----->  |  child_to_parent_out  |  -----> 
                    -------------------------       |      
                                          [0]       |
                                                    |
                                        child_to_parent_out[0]: read
        
    (4) 프로세스 포크
        새로운 자식 프로세스를 생성하여 CGI프로그램을 실행할 준비를 한다.

    (5) 자식 프로세스에서 CGI프로그램 실행
        5-1. 안쓰는 파이프 입구 닫기
            자식 프로세스에서는 아래 두개의 디스크립터를 사용하지 않는다.
                parent_to_child_in[1]
                child_to_parent_out[0]

        5-2. dup2 함수로 입출력 연결
            dup2(parent_to_child_in[0], STDIN_FILENO);
            = 현재 자식프로세스에 들어와있는 상태다.
              자식 프로세스의 표준 입력을 parent_to_child_in[0]으로 지정.
              이제부터 자식프로세스는 표준입력을 parent_to_child_in[0]에서 읽어온다.

            dup2(child_to_parent_out[1], STDOUT_FILENO);
            = 현재 자식프로세스에 들어와있는 상태다.
              자식프로세스의 표준 출력을 child_to_parent_out[1]로 지정.
              이제부터 자식프로세스는 표준출력을 child_to_parent_out[1]로 보낸다.

            dup2(child_to_parent_out[1], STDERR_FILENO);
            = 자식 프로세스의 표준 오류(STDERR)도 이제부터 터미널이 아닌 부모프로세스로 보내겠다는 소리.

        5-3. close
            복사 다 해줬으니까(= 파이프 사용 다 끝났으니) 사용했었던
            parent_to_child_in[0], child_to_parent_out[1]닫는다.

        5-4. Execute CGI with the function "execve"
            자식 프로세스에서 CGI 프로그램을 실행하기 위해 execve 함수를 호출한다.
            CGI process실행 성공하면 뒤에 남은 코드
             -> perror("CGI execution failed");
                exit (1);
                는 실행되지 않는게 정상

    (6) 출력 읽기
        자식프로세스에서 실행한 CGI프로그램의 출력을 부모 프로세스에서 읽어와 "Response part"로 전달한다.

        6-1. 안쓰는 pipe닫기
        6-2. 부모에서 자식으로 Request-body데이터 전송
            CGI프로그램으로 전달할 body데이터를 가져온다.
            write 함수를 사용해 parent_to_child_in[1]에 데이터 보낸다.
            데이터 전송끝났으니까 다 사용한 파이프 닫아준다.
        6-3. 자식 프로세스 끝날 때까지 대기
            waitpid(pid, &status, 0);
            status에는 자식프로세스의 종료상태 저장.
        6-4. 자식으로부터 읽어올 데이터들 담아둘 변수들 선언
            resultToResponse;  // 리스폰 파트로 보낼 결과값 담아둘 변수 선언
            char buffer[128];   // 자식 프로세스의 출력을 127바이트씩 임시로 저장할 버퍼 변수 선언
            ssize_t bytes_read; // 읽어온 바이트 수를 저장할 변수 선언 
        6-5. 최대 127바이트씩 계속 자식 프로세스에서 발생한 출력을 읽어와
            resultToResponse 문자열에 추가해준다.
            = 자식 프로세스가 부모 프로세스로 데이터를 전송할 때 사용하는 while loop
            = 자식 프로세스의 표준 출력을 읽어오기 위해 read함수 호출
            = 127바이트까지 읽고, 마지막 바이트는 NULL을 위해 비워둔다.

            bytes_read = read(*, *, *) > 0

            [f] read
            SUCCESS to read: return (as much as 읽은 바이트 수: bytes_read)
            NO MORE to read: return (0)
            = " > 0" 조건을 사용 -> 데이터가 남아있는 동안만 반복한다.

        6-6. response part로 보낼 결과값 저장
        6-7. 자식 프로세스 종료 상태 확인
            WIFEXITED(cgiExitStatus)
            = 자식 프로세스가 정상적으로 종료되었는지 확인하는 매크로.

            WEXITSTATUS(cgiExitStatus) != 0
            = 자식 프로세스의 종료코드 확인
            = If not '0' -> Error.
            = exit로 전달된 Error코드 반환.
*/
bool CGIExecutor::executeCGI(Request & request, Response & response)
{
    // Process (1)
    char **envCGI = createCGIEnv(request);
    for (int i = 0; envCGI[i] != NULL; i++) {
        std::cout << envCGI[i] << std::endl;
    }

    // Process (2)
    const char *path = request.getUri().c_str();
    char *argv[1];
    argv[0] = NULL;

    // Process (3)
    int parent_to_child_in[2];
    int child_to_parent_out[2];
    if (pipe(child_to_parent_out) == -1 || pipe(parent_to_child_in) == -1)
    {
        perror("Pipe creation failed");
        return (false);
    }

    // Process (4)
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("Fork failed");
        return (false);
    }

    // Process (5)
    if (pid == 0)
    {
        // 5-1
        close(child_to_parent_out[0]);
        close(parent_to_child_in[1]);

        // 5-2
        dup2(parent_to_child_in[0], STDIN_FILENO);
        dup2(child_to_parent_out[1], STDOUT_FILENO);
        dup2(child_to_parent_out[1], STDERR_FILENO);

        // 5-3
        close(child_to_parent_out[1]);
        close(parent_to_child_in[0]);

        // 5-4
        if (execve(path, argv, envCGI) == -1)
        {
            perror("CGI execution failed");
            exit(1);
        }
    }

    // Process (6)
    else
    {
        // 6-1
        close(child_to_parent_out[1]);
        close(parent_to_child_in[0]);

        // 6-2
        std::string body = request.getBody();
        write(parent_to_child_in[1], body.c_str(), body.size());
        close(parent_to_child_in[1]);

        // 6-3
        int cgiExitStatus;
        waitpid(pid, &cgiExitStatus, 0);

        // 6-4
        std::string resultToResponse;
        char buffer[128];
        ssize_t bytes_read;
    
        // 6-5
        while ((bytes_read = read(child_to_parent_out[0], buffer, sizeof(buffer) - 1)) > 0)
        {
            buffer[bytes_read] = '\0';
            resultToResponse += buffer;
        }
        close(child_to_parent_out[0]);

        // 6-6
        response.setBody(resultToResponse);

        // 6-7
        if (WIFEXITED(cgiExitStatus) && WEXITSTATUS(cgiExitStatus) != 0)
        {
            std::cerr << "CGI failed: " << WEXITSTATUS(cgiExitStatus) << "(Error code)" << std::endl;
            return false;
        }
    }
    return true;
}

/*  [ Finish ]
    [ Role ]
    CGI 스크립트를 식별.
    주어진 Param: "path"가 CGI스크립트와 관련이 있는지 판단.

    [ Background ]
    std::string::npos // C++의 std::string 클래스에서 정의된 상수.
    일반적으로 std::string::find 메서드를 사용할 때,
    찾고자 하는 문자열이 존재하지 않으면 std::string::npos를 반환
    = std::string::npos가 반환됐다는건 [f]find 에 성공했음을 의미.

    [ Process ]
    Option (1): 경로가 "/cgi-bin/"으로 시작하는지 확인
    Option (2): 경로에 ".cgi"가 포함되어 있는지 확인
    Option (3): 경로에 ".py" 이 포함되어 있는지 확인
    = 3개 중 하나라도 발견되면 true리턴 (Vice versa)
*/
bool isCGI(const std::string & path)
{
	return path.find("/cgi-bin/") == 0 || path.find(".cgi") != std::string::npos
		|| path.find(".py") != std::string::npos;
}

/*  [ Finish ]
    [ Goal & Role ]
    CGI프로그램 실행에 필요한 환경 변수를 생성(설정)해서
    executeCGI함수에서 'execve'함수를 통해
    CGI프로그램을 실행할 수 있도록 한다.

    [ Why ]
    (1) execve함수
        Param(3): "env"환경변수가 있어야 실행되는 함수다.
        환경변수를 설정해주지 않으면 함수가 실행되지 않는다.

    (2) env_map를 직접 반환하지 않고 char** env를 별도로 만들어주는 이유
        execve 함수는 환경 변수를 char** 타입으로 받는다.
        std::map형태로는 사용 할 수 없음.

    [ Process ]
    (1) "env_map"이라는 맵 생성
        HTTP 요청의 정보들을 저장할,
        CGI프로그램의 환경변수들을 저장할 맵을 생성한다.
        
    (2) CGI프로그램 작동에 필요한 환경변수들 모두 "env_map"에 저장    
        필요한 모든 소스들은 Request헤더에서 가져와서 저장한다.
       
        NAME            | INFO                      | EXAMPLE
        (KEY)           |                           | (VALUE)
        -------------------------------------------------------------------------
        REQUEST_METHOD  : 요청 메서드                   GET, POST
        SCRIPT_NAME     : 실행할 스크립트 경로 (URI)      /upload_script.py
        QUERY_STRING    : URI에서 전달되는 쿼리 문자열     ?query=apple
        PROTOCOL        : 서버가 사용중인 프로토콜 버전     HTTP/1.1
        CONTENT_TYPE    : 클라이언트가 보낸 데이터의 형식   request의 헤더부분에 나와있음
        CONTENT_LENGTH  : 클라이언트가 보낸 데이터의 크기   request의 헤더부분에 나와있음
    
    (3) 추후 execve함수가 사용 할 수 있도록 (char **)type으로 옴겨담고 리턴.

    [ Backgroun Knowledge ]
    (1) HTTP Request standard form (based on Chrome) // HTTP 요청은 표준 형식
        <php>
            (1)                 (2)                   (3)
            <HTTP_METHOD>       <URI>                 <HTTP_VERSION>
            요청하는 동작          요청하는 리소스 경로      프로토콜의 버전
        ex) "POST"              "/upload_script.py"   "HTTP/1.1"

        (1) POST매소드(동작)를 실행 할 때
        (3) HTTP/1.1 프로토콜을 기준으로
        (2) /upload_script.py 경로에 있는 스크립트를 찾아서 실행해라.

    (2) QUERY_STRING
        웹 브라우저가 서버에 요청을 보낼 때 URL뒤에 추가하는(디테일한) 정보
        ex) https://example.com/search?query=apple&limit=10
                                      ---------------------
                                      '?' 뒤에 오는 부분을 QUERY_STRING이라고 한다.

        Q: QUERY_STRING을 왜 별도로 처리해줘야 하는가
        A:'?'를 포함한 요청이 들어올 경우를 처리해주기 위해

    (3) CONTENT_TYPE & CONTENT_LENGTH
        Chrome웹 브라우저는 HTTP 요청을 보낼 때 "헤더" 부분에
        "Content-Type" & "Content-Length"를 포함하는것이 일반적이다.
        = 크롬에서 정한 약속이다.
    
    (4) std::map
        "Data structure" in C++
        Associative container (KEY + VALUE)
        = 이진 검색 트리를 기반으로 하여 내부적으로 데이터를 저장하고 정렬.

    (5) std::map<std::string, std::string>::iterator
        std::map이 제공하는 반복자(iterator) 타입을 의미
        = 이 반복자를 사용하면 map에 있는 요소들을 하나씩 순회할 수 있다.
    
    (6) first, second
        std::map 자료구조에서 각각 KEY, VALUE를  의미.

    [ Detail ]
    (1) char** env = new char*[env_map.size() + 1];

        char *[env_map.size():6 + 1: NULL]
        char *[7]

        char *env[0] -> "REQUEST_METHOD=POST"
        char *env[1] -> "SCRIPT_NAME=/upload_script.py"
        char *env[2] -> "QUERY_STRING=query=apple"
        char *env[3] -> "PROTOCOL=HTTP/1.1"
        char *env[4] -> "CONTENT_TYPE=multipart/form-data"
        char *env[5] -> "CONTENT_LENGTH=1234"

        char **env -> env[0], env[1], ..., env[5]

    (2) for (std::map<std::string, std::string>::iterator it
                = env_map.begin(); it != env_map.end(); ++it)

        std::map 자료구조를 순회할 때 자주 사용하는 패턴.

    (3) std::string var = it->first + "=" + it->second;

        var = REQUEST_METHOD=POST           // size: 19
        env[0] = new char[20];             
        std::strcpy(env[0], var.c_str());   // Result: env[0] = "REQUEST_METHOD=POST\0"
*/
char** CGIExecutor::createCGIEnv(Request& request)
{
    // Process (1)
    std::map<std::string, std::string> env_map;

    // Process (2)
    env_map["REQUEST_METHOD"] = request.getMethod().substr(0);
    env_map["SCRIPT_NAME"] = request.getUri();
    env_map["QUERY_STRING"] = request.buildQueryString();
    env_map["PROTOCOL"] = request.getVersion();
    env_map["CONTENT_TYPE"] = request.getHeaderOrDefault("Content-Type", "");
    env_map["CONTENT_LENGTH"] = request.getHeaderOrDefault("Content-Length", "0");

    // Process (3)
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
