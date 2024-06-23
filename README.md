# webserv

#### Summary

This project is about writing a HTTP server in `C++ 98`. The server should be testable with actual browsers and others, like `wget` and `curl`. This project is aimed to learn about HTTP, one of the most used protocols on the internet, and implement compatible program with the protocol.

## Introduction

The **Hypertext Transfer Protocol** (HTTP) is an application protocol for distributed, collaborative, hypermedia information systems.

HTTP is the foundation of data communication for the Wolrd Wide Web, where hypertext documents include hyperlinks to other resources that the user can easily access. For example, by a mouse click or by tapping the screen in a web browser.

HTTP was developed to facilitate hypertext and the World Wide Web.

The primary function of a web server is to store, process, and deliver web pages to clients. The communication between client and server takes place using the Hypertext Transfer Protocol (HTTP).

Pages delivered are most frequently HTML documents, which may include images, style sheets, and scripts in addition to the text content.

Multiple web servers may be used for a high-traffic website.

A user agent, commonly a web browser or web crawler, initiates communication by requesting a specific resource using HTTP and the server responses with the content of that resource or an error message if unable to do so. The resource is typically a real file on the server's secondary storage, but this is not necessarily the case and depends on how the webserver is implemented.

While the primary function is to serve content, full implementation of HTTP also includes ways of receiving content from clients. This feature is used for submitting web forms, including the uploading of files.

---

## General rules

- The program shoudl not crash in any circumstances, and should not quit unexpectedly.
  - even when it runs out of memory.
- Use `Makefile` to compile all source files.
  - at least contain following rules:
    - `$(NAME)`, `all`, `clean`, `fclean` and `re`.
- Use `c++` to compile code and flags with `-Wall -Wextra -Werror`
- Code must comply with **C++ 98 standard**. It should still compile flag with `-std=c++98`
- Try to develop using the most `C++` features. For example, choose `<cstring>` over `<string.h>`.
- Any external library and `Boost` libraries are forbidden.

---

## Goals

- Implement a HTTP webserver comply with `HTTP/1.1`.
- Implement the server to communicate in methods with GET, HEAD, DELETE, POST.
- Implement a server that config with dedicated configuration file which is similar with how the **NGINX** do.
- Support multi-port and routing.
- Be able to serve full static webpage.
- Use socket programming, asynchronous communication, non-block I/O
- ...?

---

## Features (ing)

### Mandatory (ing)

- Multi-port & Multi-host
- IO Multiplex
- Implement using `select()` or `poll()` or any equivalent.
- ...

### Configuration (ing)

- Support Multi-port & Multi-host
- Have default error pages
- Able to limit the client body
- Able to config root dir to serve
- Able to config default file to serve
- Able to authorize http method to specific route

### Basic (ing)

- Support GET POST DELETE methods based on HTTP/1.1
- Able to upload and download a file.

### for Browser (ing)

- Able to communicate with dedicated browsers
  - Firefox
  - Chrome
  - wget
  - curl
  - etc.
- Check the pair of request header and reponse header respond as a pair
- Able to serve full static website
  - html
  - css
  - js
- Redirection(?)

### Test (ing)

- Use [siege](https://github.com/JoeDog/siege) or [plow](https://github.com/six-ddc/plow) to conduct load test
- Check whether connections are maintained normally and nothing is terminated
- Not allow auto-restart of the server
- Minimize increase of mem-usage over an idle status
- Check mem-leak

## Coding Convention (ing)

- Comment & documentation with `///`
- Class
  - Naming private attribute with `_`
    - ex. `_somename = "abc";`
- Utilize standard libraries as much as possible
  - std::map
  - std::vector
  - std::set
  - etc.
