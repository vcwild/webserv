#include "dirent.h"
#include "response.hpp"
#include <algorithm>
#include <iostream>
#include <string>

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

static int isFile( std::string path )
{
    std::ifstream indexFile( path.c_str() );

    if ( indexFile.is_open() ) {
        return TRUE;
    } else {
        return FALSE;
    }
}

static std::string getFileExtension( std::string path )
{
    return path.substr( path.find_last_of( "." ) + 1 );
}

std::vector<std::string> split( std::string text, char separator = ' ' )
{
    std::string              str;
    std::stringstream        ss( text );
    std::vector<std::string> result;

    while ( getline( ss, str, separator ) ) {
        result.push_back( str );
    }

    return result;
}

int ft::Response::canAutoIndex( std::string path )
{
    logger.info( "Checking if autoindex is enabled" );
    std::string cleanPath = split( path, '/' )[1];

    // iterate over server_conf.routes and check if autoindex is enabled
    for ( std::vector<Route>::iterator it = server_conf.routes.begin();
          it != server_conf.routes.end();
          ++it ) {
        logger.debug( "Location: " + it->location_dir );
        if ( it->location_dir == ( "/" + cleanPath ) ) {
            logger.info( "Autoindex: " + it->autoindex );
            if ( it->autoindex == "on" ) {
                logger.info( "Autoindex is enabled" );
                return TRUE;
            } else {
                logger.info( "Autoindex is disabled" );
                return FALSE;
            }
        }
    }

    return TRUE;
}

void ft::Response::handleGet()
{
    if ( isDirectory( server_conf.root_dir + request.uri ) ) {
        logger.info( "Request is for a directory" );
        if ( canAutoIndex( request.uri ) ) {
            logger.info( "Allowed to auto index" );
            std::string body;
            // createDirectoryListing( getPath( request.uri ), body );
            setContentType( mime_types.getMimeType( ".html" ) );
            setBody( body );
            setStatusCode( status_codes.getStatusCode( 200 ) );
        } else {
            callErrorPage( body, server_conf.error_page[1] );
        }
    } else if ( isFile( getPath( request.uri ) ) ) {
        logger.info( "Request is for a file" );
        readFromAFile( getPath( request.uri ), body );
        setContentType( mime_types.getMimeType(
            "." + getFileExtension( getPath( request.uri ) ) ) );
        setStatusCode( status_codes.getStatusCode( 200 ) );
    } else {
        callErrorPage( body, server_conf.error_page[1] );
    }
}
