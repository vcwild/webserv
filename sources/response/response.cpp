#include "response.hpp"

ft::Response::Response() {}

ft::Response::~Response() {}

int ft::Response::isValidMethod( std::string method )
{
    if ( method == "GET" || method == "POST" || method == "DELETE" ) {
        return TRUE;
    }
    return FALSE;
}

ft::Response::Response( Request request, Config server_conf ) :
    request( request ), server_conf( server_conf )
{

    if ( !isValidMethod( request.method ) ) {
        setStatusCode( "405 Method Not Allowed" );
        setBody( "Method not allowed" );
        return;
    }

    if ( request.method == "GET" ) {

        handleGet( request );
    }

    if ( request.method == "POST" ) {
        setStatusCode( "200 OK" );
        setBody( "POST" );
    }

    if ( request.method == "DELETE" ) {
        setStatusCode( "200 OK" );
        setBody( "DELETE" );
    }
}

int ft::Response::getContentLength() { return this->body.length(); }

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

    logger.debug( "Response: " + response );

    return response;
}
