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

static void readFromAFile( std::string path, std::string &body )
{
    std::ifstream file( path.c_str() );
    if ( file.is_open() ) {
        std::string line;
        while ( getline( file, line ) ) {
            body.append( line );
        }
        file.close();
    }
}

void ft::Response::handleGet()
{
    std::string   index_path = getPath( request.uri );
    std::ifstream index_file( index_path.c_str() );

    std::string extension
        = index_path.substr( index_path.find_last_of( "." ) + 1 );
    setContentType( mime_types.getMimeType( "." + extension ) );

    if ( index_file.is_open() ) {
        readFromAFile( index_path, body );
        setStatusCode( status_codes.getStatusCode( 200 ) );
    } else {
        // Load default 404 page 404.html
        std::string error_path = getPath( "/404.html" );
        readFromAFile( error_path, body );
        setStatusCode( status_codes.getStatusCode( 404 ) );
    }
}