#include "response.hpp"

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

void ft::Response::handleDelete()
{
    std::string   index_path = getPath( request.uri );
    std::ifstream index_file( index_path.c_str() );

    if ( index_file.is_open() ) {
        std::string error_path = getPath( "/401.html" );
        readFromAFile( error_path, body );
        setStatusCode( status_codes.getStatusCode( 401 ) );
        setContentType( "text/html" );
        index_file.close();
    } else {
        std::string error_path = getPath( "/404.html" );
        readFromAFile( error_path, body );
        setContentType( "text/html" );
        setStatusCode( status_codes.getStatusCode( 404 ) );
    }
}