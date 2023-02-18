#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "utils.hpp"
#include "webserv.hpp"
#include <cerrno>
#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0

/**
 * @brief Response example
 * HTTP/1.1 202 Accepted
 * Content-Type: text/plain
 * Content-Length: 11

 * Maybe I'll
 *
 */

namespace ft {

class Response {

private:
    Request request;

public:
    Response();
    Response( Request request, Config server_conf );
    ~Response();

    std::string statusCode;
    std::string body;
    Config      server_conf;

    int         getContentLength();
    void        setStatusCode( std::string code );
    void        setBody( std::string body );
    std::string makeResponse();
    int         isValidMethod( std::string method );
    std::string getPath( std::string uri );
    void        handleGet();
    void        handlePost();
};

}
#endif
