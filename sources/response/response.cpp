#include "response.hpp"
#include "utils.hpp"

ft::Response::Response() {}

ft::Response::Response( Request request ) : request( request )
{
    setStatusCode( "200 OK" );
    setBody( "Hello World" );
}

ft::Response::~Response() {}

int ft::Response::getContentLength() { return body.length(); }

void ft::Response::setStatusCode( std::string code )
{
    this->statusCode = code;
}

void ft::Response::setBody( std::string body ) { this->body = body; }

std::string ft::Response::makeResponse()
{
    std::string response;
    response.append( "HTTP/1.1 " );
    response.append( statusCode );
    response.append( "\r\n" );
    response.append( "Content-Type: text/html\r\n" );
    response.append( "Content-Length: " );
    response.append( NumberToString( getContentLength() ) );
    response.append( "\r\n\r\n" );
    response.append( body );

    logger.info( "Response: " + response );

    return response;
}
