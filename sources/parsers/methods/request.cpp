#include "webserv.hpp"
#include <algorithm>

std::string &removeSpecialCharacters( std::string       &str,
                                      const std::string &chars )
{
    for ( size_t i = 0; i < chars.length(); i++ ) {
        str.erase( std::remove( str.begin(), str.end(), chars[i] ), str.end() );
    }
    return str;
}

std::string &findSubstring( std::string &str, const std::string &chars )
{
    size_t pos = str.find( chars );
    if ( pos != std::string::npos ) {
        str.erase( 0, pos + chars.length() );
    }
    return str;
}

Request::Request()
{
    method        = "";
    host          = "";
    port          = "";
    uri           = "";
    version       = "";
    body          = "";
    content_type  = "";
    user_agent    = "";
    authorization = "";
    query         = "";
    cgi_path      = "";
}

Request::Request( const char *buf )
{
    std::stringstream ss( buf );
    std::string       line;

    setBody( buf );
    while ( std::getline( ss, line ) ) {
        if ( ( line ).find( GET ) != std::string::npos
             || ( line ).find( POST ) != std::string::npos
             || ( line ).find( DELETE ) != std::string::npos ) {
            setMethod( line );
        } else if ( ( line ).find( HOST_KEY ) != std::string::npos ) {
            setHost( line );
        } else if ( ( line ).find( USER_AGENT_KEY ) != std::string::npos ) {
            setUserAgent( line );
        } else if ( ( line ).find( ACCEPT_KEY ) != std::string::npos ) {
            setAccept( line );
        } else if ( ( line ).find( CONTENT_LENGTH_KEY ) != std::string::npos ) {
            setConcentLength( line );
        } else if ( ( line ).find( CONTENT_TYPE_KEY ) != std::string::npos ) {
            setContentType( line );
        }
    }
}

void Request::setUserAgent( std::string &line )
{
    std::vector<std::string> user_agent_line
        = split( removeSpecialCharacters( line, " \r" ), ":" );

    user_agent = user_agent_line[1];
}

void Request::setHost( std::string &line )
{
    std::vector<std::string> host_line
        = split( removeSpecialCharacters( line, " \r" ), ":" );

    host = host_line[1];
    port = host_line[2];
}

void Request::setMethod( std::string &line )
{
    std::vector<std::string> request_line
        = split( removeSpecialCharacters( line, "\r" ), " " );

    method = request_line[0];

    if ( ( request_line[1] ).find( "?" ) != std::string::npos ) {

        logger.debug( "Request has query" );
        std::vector<std::string> uri_line
            = split( removeSpecialCharacters( request_line[1], "\r" ), "?" );
        logger.debug( "URI: " + uri_line[0] );
        logger.debug( "Query: " + uri_line[1] );
        uri   = uri_line[0];
        query = uri_line[1];
    } else {
        uri = request_line[1];
    }

    version = request_line[2];

    if ( ( uri ).find( CGI_PATH ) != std::string::npos ) {
        cgi_path = "./www" + uri;
    }
}

void Request::setAccept( std::string &line )
{
    std::vector<std::string> accept_line
        = split( removeSpecialCharacters( line, " \r" ), ":" );

    accept = accept_line[1];
}

void Request::setConcentLength( std::string &line )
{
    std::vector<std::string> content_length_line
        = split( removeSpecialCharacters( line, " \r" ), ":" );

    content_length = content_length_line[1];
}

void Request::setContentType( std::string &line )
{
    std::vector<std::string> content_type_line
        = split( removeSpecialCharacters( line, " \r" ), ":" );

    content_type = content_type_line[1];
}

void Request::setBody( const char *buf )
{
    std::string tmp( buf );

    std::string substr = findSubstring( tmp, "\r\n\r\n" );
    body               = substr;
}

void Request::display()
{
    logger.debug( "Method: " + method );
    logger.debug( "Host: " + host );
    logger.debug( "Port: " + port );
    logger.debug( "URI: " + uri );
    logger.debug( "Version: " + version );
    if ( content_type != "" )
        logger.debug( "Content-Type: " + content_type );
    logger.debug( "User-Agent: " + user_agent );
    if ( authorization != "" )
        logger.debug( "Authorization: " + authorization );
    if ( query != "" )
        logger.debug( "Query: " + query );
    if ( cgi_path != "" )
        logger.debug( "CGI-Path: " + cgi_path );
    if ( body != "" )
        logger.debug( "Body: " + body );
}
