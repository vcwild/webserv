#include "response.hpp"

ft::Response::Response() {}

ft::Response::~Response() {}

int ft::Response::isValidMethod( std::string method )
{
    for ( std::vector<std::string>::iterator it
          = server_conf.allowed_method.begin(),
          end = server_conf.allowed_method.end();
          it != end;
          ++it ) {
        if ( method == *it ) {
            return TRUE;
        }
    }
    logger.warning( "Invalid method invoked by client: " + method );
    return FALSE;
}

int ft::Response::isLocation( std::string path )
{
    std::string cleanPath = ftSplit( path, '/' )[1];

    for ( std::vector<Route>::iterator it = server_conf.routes.begin();
          it != server_conf.routes.end();
          ++it ) {
        if ( it->location_dir == ( "/" + cleanPath ) ) {
            using_route = *it;
            return TRUE;
        }
    }
    return FALSE;
}

ft::Response::Response( Request request, Config server_conf ) :
    request( request ), server_conf( server_conf )
{
    if ( isLocation( request.uri ) ) {
        if ( !isValidMethod( using_route.allow_methods ) ) {
            setStatusCode( status_codes.getStatusCode( 405 ) );
            setBody( "Method not allowed \n" );
            return;
        }
    }

    if ( !isValidMethod( request.method ) ) {
        setStatusCode( status_codes.getStatusCode( 405 ) );
        setBody( "Method not allowed \n" );
        return;
    }

    if ( request.method == "GET" ) {
        handleGet();
    }

    if ( request.method == "DELETE" ) {
        handleDelete();
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

    // Check body limit and truncate if necessary
    if ( server_conf.client_max_body_size
         && getContentLength() > server_conf.client_max_body_size ) {
        body = body.substr( 0, server_conf.client_max_body_size );
    }

    response.append( body );

    return response;
}
