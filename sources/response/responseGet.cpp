#include "dirent.h"
#include "response.hpp"

/*
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
    logger.error( "Could not open directory" );
*/

std::string ft::Response::getPath( std::string uri )
{
    std::string rootDir = server_conf.root_dir;
    std::string path    = rootDir + uri;

    // Check if request is for a directory
    path.append( server_conf.index[0] );

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

// function to check if the request is for a directory or a file
static int isDirectory( std::string path )
{
    DIR *dir = opendir( path.c_str() );
    if ( dir ) {
        closedir( dir );
        return TRUE;
    } else if ( ENOTDIR == errno ) {
        return FALSE;
    } else {
        return FALSE;
    }
}

void ft::Response::handleGet()
{
    std::string   indexPath = getPath( request.uri );
    std::ifstream indexFile( indexPath.c_str() );
    std::string   extension
        = indexPath.substr( indexPath.find_last_of( "." ) + 1 );

    setContentType( mime_types.getMimeType( "." + extension ) );
    logger.info( "autoindex: " + server_conf.autoindex );
    logger.info( "request uri: " + request.uri );

    // log all routes from server_conf
    for ( std::vector<Route>::iterator it = server_conf.routes.begin();
          it != server_conf.routes.end();
          ++it ) {
        logger.info( "route: " + it->location_dir );
    }

    logger.info( "routes size: "
                 + NumberToString( server_conf.routes.size() ) );

    if ( isDirectory( server_conf.root_dir + indexPath )
         && server_conf.autoindex == "on" ) {
        std::string errorPath = getPath( "/404.html" );
        logger.info( "Request is for a directory" );
        callErrorPage( body, server_conf.error_page[1] );
    } else if ( indexFile.is_open() ) {
        readFromAFile( indexPath, body );
        setStatusCode( status_codes.getStatusCode( 200 ) );
    } else {
        callErrorPage( body, server_conf.error_page[1] );
    }
}
