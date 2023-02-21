#include "response.hpp"
#include <algorithm>

ft::Response::Response() {}

ft::Response::~Response() {}

int ft::Response::isValidMethod( std::string method )
{
    if ( ( method == "GET" || method == "POST" || method == "DELETE" )
         && std::find( server_conf.allowed_method.begin(),
                       server_conf.allowed_method.end(),
                       method )
             != server_conf.allowed_method.end() ) {
        return TRUE;
    }
    return FALSE;
}

ft::Response::Response( Request request, Config server_conf ) :
    request( request ), server_conf( server_conf )
{

    if ( !isValidMethod( request.method ) ) {
        setStatusCode( status_codes.getStatusCode( 405 ) );
        setBody( "Method not allowed" );
        return;
    }

    if ( request.method == "GET" ) {
        handleGet();
    }

    if ( request.method == "POST" ) {
        handlePost();
    }

    if ( request.method == "DELETE" ) {
        setStatusCode( status_codes.getStatusCode( 200 ) );
        setBody( "DELETE" );
    }
}

int ft::Response::getContentLength() { return this->body.length(); }

void ft::Response::setStatusCode( std::string code )
{
    this->statusCode = code;
}

void ft::Response::setContentType( std::string type )
{
    this->_contentType = type;
}

std::string ft::Response::getContentType() { return this->_contentType; }

void ft::Response::setBody( std::string body ) { this->body = body; }

std::string ft::Response::makeResponse()
{
    std::string response;
    response.append( "HTTP/1.1 " );
    response.append( statusCode );
    response.append( "\r\n" );
    response.append( "Content-Type: " );
    response.append( getContentType() );
    response.append( "\r\n" );
    response.append( "Content-Length: " );
    response.append( NumberToString( getContentLength() ) );
    response.append( "\r\n" );
    response.append( "Connection: keep-alive" );
    response.append( "\r\n\r\n" );
    response.append( body );

    logger.debug( "Content Type: " + getContentType() );
    logger.debug( "Content Length: " + NumberToString( getContentLength() ) );
    logger.debug( "Body: " + body );

    return response;
}
