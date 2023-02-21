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

static int readFromAFile( std::string path, std::string &body )
{
    std::ifstream file( path.c_str() );
    if ( file.is_open() ) {
        std::string line;
        while ( getline( file, line ) ) {
            body.append( line );
        }
        file.close();
        return TRUE;
    }
    return FALSE;
}

void ft::Response::callErrorPage( std::string &body, std::string error_page )
{
    std::string   parsedErrorPage = error_page.erase( 0, 1 );
    std::string   errorPath       = getPath( parsedErrorPage );
    std::ifstream errorFile( errorPath.c_str() );

    if ( errorFile.is_open() ) {
        readFromAFile( errorPath, body );
        setContentType( mime_types.getMimeType( ".html" ) );
        setStatusCode( status_codes.getStatusCode( 404 ) );
    } else {
        std::string errorPath = getPath( "/404.html" );
        setContentType( mime_types.getMimeType( ".html" ) );
        readFromAFile( errorPath, body );
        setStatusCode( status_codes.getStatusCode( 404 ) );
    }
}

void ft::Response::handleGet()
{
    std::string   indexPath = getPath( request.uri );
    std::ifstream indexFile( indexPath.c_str() );
    std::string   extension
        = indexPath.substr( indexPath.find_last_of( "." ) + 1 );

    setContentType( mime_types.getMimeType( "." + extension ) );

    if ( indexFile.is_open() ) {
        readFromAFile( indexPath, body );
        setStatusCode( status_codes.getStatusCode( 200 ) );
    } else {
        callErrorPage( body, server_conf.error_page[1] );
    }
}
