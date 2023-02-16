#ifndef RESPONSE_HPP
#define RESPONSE_HPP

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
    Response( Request request );
    ~Response();

    std::string statusCode;
    std::string body;

    int         getContentLength();
    void        setStatusCode( std::string code );
    void        setBody( std::string body );
    std::string makeResponse();
    std::string getPath( std::string uri );
    int         isValidMethod( std::string method );
    void        handleGet( Request request );
};

}
#endif
