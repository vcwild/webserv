#include "response.hpp"

ft::Response::Response() {}

ft::Response::~Response() {}

int ft::Response::isValidMethod( std::string method )
{

    ;

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

ft::Response::Response( Request request, Config server_conf ) :
    request( request ), server_conf( server_conf )
{

    if ( !isValidMethod( request.method ) ) {
        setStatusCode( status_codes.getStatusCode( 405 ) );
        setBody( "Method not allowed" );
        return;
    }

    if ( request.cgi_path != "" ) {
        Cgi_handler cgi( request );
        cgi.run();
        body = cgi.get_response_body();
        setStatusCode( status_codes.getStatusCode( 200 ) );
        return;
    }

    if ( request.method == "GET" ) {
        handleGet();
    }

    if ( request.method == "POST" ) {
        handlePost();
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
    response.append( "\r\n\r\n" );
    response.append( body );

    return response;
}
