#include "dirent.h"
#include "response.hpp"

static void createDirectoryListing( std::string path, std::string &body )
{
    DIR           *dir;
    struct dirent *ent;
    if ( ( dir = opendir( path.c_str() ) ) != NULL ) {
        while ( ( ent = readdir( dir ) ) != NULL ) {
            body.append( ent->d_name );
            body.append( "" );
        }
        closedir( dir );
    } else {
        /* could not open directory */
        logger.error( "Could not open directory" );
    }
}

std::string ft::Response::getPath( std::string uri )
{
    std::string rootDir = server_conf.root_dir;
    std::string path    = rootDir + uri;

    // Check if request is for a directory
    if ( server_conf.autoindex == "on" ) {
        // Responde with a directory listing
        createDirectoryListing( path, body );
        setContentType( mime_types.getMimeType( ".html" ) );
        setStatusCode( status_codes.getStatusCode( 200 ) );
    } else {
        path.append( server_conf.index[0] );
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
        setContentType( mime_types.getMimeType( ".html" ) );
        setBody( "<html><body><h1>404 Not Found</h1></body></html>" );
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

    if ( extension == "py" ) {
        Cgi_handler cgi( request );
        cgi.run();
        setContentType( mime_types.getMimeType( ".html" ) );
        body = cgi.get_response_body();
        setStatusCode( status_codes.getStatusCode( 200 ) );
        logger.debug( "CGI response: " + body );
        return;
    }

    if ( indexFile.is_open() ) {
        readFromAFile( indexPath, body );
        setStatusCode( status_codes.getStatusCode( 200 ) );
    } else {
        callErrorPage( body, server_conf.error_page[1] );
    }
}
