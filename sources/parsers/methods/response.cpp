#include "webserv.hpp"
#include <iostream>
#include <map>
#include <sstream>
#include <string>

static std::map<std::string, std::string>
parseHeader( std::istringstream &sstream )
{
    std::map<std::string, std::string> header_map;
    std::string                        line;

    // Get the first line
    std::getline( sstream, line );
    header_map[STATUS_KEY] = removeSpecialCharacters( line, "\r" );

    // Get the remaining lines
    while ( std::getline( sstream, line ) ) {
        std::string key;
        std::string value;

        // Find the first colon in the line
        size_t colon_pos = line.find( ":" );
        if ( colon_pos == std::string::npos ) {
            continue;
        }

        // Extract the key and value
        key             = line.substr( 0, colon_pos );
        std::string tmp = line.substr( colon_pos + 2 );
        value           = removeSpecialCharacters( tmp, "\r" );

        // Add the key-value pair to the map
        header_map[key] = value;
    }

    return header_map;
}

void Response::setBody( char *buf )
{
    std::string tmp( buf );

    std::string substr = findSubstring( tmp, "\r\n\r\n" );
    // if substring is the same as the original string means
    // that there is no body
    if ( tmp == substr )
        substr = "";
    body = substr;
}

Response::Response()
{
    version        = ""; // HTTP/1.1
    status_code    = ""; // 200
    status_message = ""; // OK
    content_type   = ""; // text/html
    content_length = ""; // 1234
    date           = ""; // Tue, 15 Nov 1994 08:12:31 GMT
    server         = ""; // Apache/2.4.1 (Unix)
    connection     = ""; // Keep-Alive
    accept_ranges  = ""; // bytes
    keep_alive     = "";
    body           = "";
}

Response::Response( char *buf )
{
    setBody( buf );
    std::istringstream                 sstream( buf );
    std::map<std::string, std::string> header_map = parseHeader( sstream );
    setFirstLine( header_map[STATUS_KEY] );
    accept_ranges  = header_map[ACCEPT_RANGES_KEY];
    connection     = header_map[CONNECTION_KEY];
    content_length = header_map[CONTENT_LENGTH_KEY];
    content_type   = header_map[CONTENT_TYPE_KEY];
    date           = header_map[DATE_KEY];
    keep_alive     = header_map[KEEP_ALIVE_KEY];
    server         = header_map[SERVER_KEY];
}

void Response::setFirstLine( std::string &line )
{
    std::vector<std::string> response_line = split( line, " " );

    version        = response_line[0];
    status_code    = response_line[1];
    status_message = response_line[2];
}
