#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "cgi_handler.hpp"
#include "mimetypes.hpp"
#include "statusCodes.hpp"
#include "utils.hpp"
#include "webserv.hpp"
#include <cerrno>
#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>

#include <algorithm>
#include <iostream>
#include <string>

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
    Route       using_route;

    int         getContentLength();
    void        setStatusCode( std::string code );
    void        setBody( std::string body );
    void        setContentType( std::string type );
    std::string getContentType();
    std::string makeResponse();
    int         isValidMethod( std::string              method,
                               std::vector<std::string> allowed_methods );
    void createDirectoryListingIntoHTML( std::string path, std::string &body );
    std::string getPath( std::string uri );
    int         canAutoIndex( std::string path );
    void        handleGet();
    void        handlePost();
    void        handleDelete();
    int         isLocation( std::string path );
};
}
#endif
