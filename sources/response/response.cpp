#include "response.hpp"
#include "utils.hpp"

ft::Response::Response() {}

std::string ft::Response::getPath( std::string uri )
{
    std::string path = "./www" + uri;
    if ( path[path.length() - 1] == '/' ) {
        path.append( "index.html" );
    }
    return path;
}

int ft::Response::isValidMethod( std::string method )
{
    if ( method == "GET" || method == "POST" || method == "DELETE" ) {
        return TRUE;
    }
    return FALSE;
}

void ft::Response::handleGet( Request request )
{
    std::string   index_path = getPath( request.uri );
    std::ifstream index_file( index_path.c_str() );
    if ( index_file.is_open() ) {
        std::string line;
        while ( getline( index_file, line ) ) {
            body.append( line );
        }
        index_file.close();
        setStatusCode( "200 OK" );
    } else {
        setStatusCode( "404 Not Found" );
        setBody( "404 Not Found" );
    }
}

ft::Response::Response( Request request ) : request( request )
{
    if ( !isValidMethod( request.method ) ) {
        setStatusCode( "405 Method Not Allowed" );
        setBody( "Method not allowed" );
        return;
    }

    if ( request.method == "GET" ) {
        handleGet( request );
    }
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
