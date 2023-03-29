#include "response.hpp"

std::string ft::Response::generateFileName()
{
    std::string rootDir = server_conf.root_dir;

    std::string file_path = rootDir + "/uploads/";

    for ( int i = 0; i < 10; i++ ) {
        file_path += static_cast<char>( rand() % 26 + 97 );
    }

    file_path += ".txt";
    return file_path;
}

void ft::Response::handlePost()
{
    std::string path = getPath( request.uri );

    if ( request.content_type != "text/plain" ) {
        setStatusCode( status_codes.getStatusCode( 200 ) );
        return;
    }

    std::string   file_path = generateFileName();
    std::ofstream file( file_path.c_str() );

    if ( !file.is_open() ) {
        setStatusCode( status_codes.getStatusCode( 500 ) );
        setContentType( mime_types.getMimeType( ".html" ) );
        setBody(
            "<html><body><h1>500 Internal Server Error</h1></body></html>" );
        return;
    }

    file << request.body;
    file.close();
    setStatusCode( status_codes.getStatusCode( 201 ) );
    setContentType( mime_types.getMimeType( ".html" ) );
    setBody( "<html><body><h1>201 Created</h1></body></html>" );
}