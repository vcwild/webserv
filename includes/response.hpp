#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "mimetypes.hpp"
#include "statusCodes.hpp"
#include "utils.hpp"
#include "webserv.hpp"
#include <cerrno>
#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0

static MimeType   mime_types;
static StatusCode status_codes;

namespace ft {

class Response {

private:
    Request     request;
    std::string _contentType;
    void        callErrorPage( std::string &body, std::string error_page );

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
    void        setContentType( std::string type );
    std::string getContentType();
    std::string makeResponse();
    int         isValidMethod( std::string method );
    std::string getPath( std::string uri );
    void        handleGet();
    void        handlePost();
};

}
#endif
