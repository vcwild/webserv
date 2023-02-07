#include "response.hpp"

Response::Response() {}

Response::~Response() {}

int Response::getContentLength() { return body.length(); }

void Response::setStatusCode( std::string code ) { this->statusCode = code; }

void Response::setBody( std::string body ) { this->body = body; }

void Response::makeResponseBody( std::string body )
{
    std::string response;

    response.append( "HTTP/1.1 " );
    response.append( statusCode );
    response.append( "\r\n" );
    response.append( "Content-Type: text/html\r\n" );
    response.append( "Content-Length: " );
    response.append( std::to_string( getContentLength() ) );
    response.append( "\r\n\r\n" );
    response.append( body );
}