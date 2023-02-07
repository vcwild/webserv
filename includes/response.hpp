#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "webserv.hpp"
#include <cerrno>
#include <fcntl.h>
#include <iostream>

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

class ResponseC {

private:
public:
    ResponseC();
    ~ResponseC();

    std::string statusCode;
    std::string body;

    int  getContentLength();
    void setStatusCode( std::string code );
    void setBody( std::string body );
    void makeResponseBody( std::string body );
};

#endif
