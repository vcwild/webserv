#include "response.hpp"

std::string ft::Response::getPath( std::string uri )
{
    std::string rootDir = server_conf.root_dir;
    std::string path    = rootDir + uri;
    if ( path[path.length() - 1] == '/' ) {
        path.append( "index.html" );
    }
    return path;
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
        // Show default 404 page if none are provided
        setStatusCode( "404" );
        setBody( "404 Not Found" );
    }
}