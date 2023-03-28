#include "response.hpp"

ft::Response::Response() {}

ft::Response::~Response() {}

int ft::Response::isValidMethod( std::string              method,
                                 std::vector<std::string> allowed_methods )
{
    for ( std::vector<std::string>::iterator it  = allowed_methods.begin(),
                                             end = allowed_methods.end();
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
    if (server_conf.server_name != request.host) {
   
        setStatusCode("400 Bad Request");
        setContentType("text/plain");
        setBody("Invalid Host");
        return; 
    }

   if (server_conf.client_max_body_size != -1 && static_cast<int>(request.body.size()) > static_cast<int>(server_conf.client_max_body_size)) {
        setStatusCode("413 Payload Too Large");
        setContentType("text/plain");
        setBody("Request body size exceeds the limit.");
        return;
    }

    if ( request.uri != "/" && request.uri != ""
         && isLocation( request.uri ) ) {
        if ( !isValidMethod( request.method, using_route.allow_methods ) ) {
            setStatusCode( status_codes.getStatusCode( 405 ) );
            setBody( "Method not allowed \n" );
            return;
        }
    }

    if ( !isValidMethod( request.method, server_conf.allowed_method ) ) {
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

void ft::Response::setLocation( std::string location )
{
    this->location = location;
}

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

    if ( location.length() > 0 ) {
        response.append( "Location: " );
        response.append( location );
        response.append( "\r\n" );
    }

   


    response.append( body );

    return response;
}
