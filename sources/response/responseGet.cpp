#include "dirent.h"
#include "response.hpp"
#include <algorithm>
#include <iostream>
#include <string>

void ft::Response::createDirectoryListingIntoHTML( std::string  path,
                                                   std::string &body )
{
    DIR           *dir;
    struct dirent *ent;
    if ( ( dir = opendir( path.c_str() ) ) != NULL ) {
        body.append( "<html><head><title>Index of " + path
                     + "</title></head>"
                       "<body><h1>Index of "
                     + path + "</h1>" );
        while ( ( ent = readdir( dir ) ) != NULL ) {
            std::string name = ent->d_name;
            if ( name != "." && name != ".." ) {
                body.append( "<a href=\"" + request.uri + "/" + name + "\">"
                             + name + "</a><br>" );
            }
        }
        body.append( "</body></html>" );
        closedir( dir );
    } else {
        setContentType( mime_types.getMimeType( ".html" ) );
        setBody( "<html><body><h1>404 Not Found Directory</h1></body></html>" );
        setStatusCode( status_codes.getStatusCode( 404 ) );
    }
}

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
    std::string cleanPath = split( path, '/' )[1];

    for ( std::vector<Route>::iterator it = server_conf.routes.begin();
          it != server_conf.routes.end();
          ++it ) {
        if ( it->location_dir == ( "/" + cleanPath ) ) {
            if ( it->autoindex == "on" ) {
                return TRUE;
            } else {
                return FALSE;
            }
        }
    }
    return TRUE;
}

static void makeRedirectFile( std::string &body, std::string location )
{
    body.append( "<html><head><title>301 Moved Permanently</title>"
                 "<script>window.location.replace('"
                 + location + "');</script>"
                 + "</head><body><h1>Moved Permanently</h1><p>The "
                 + "document has moved <a href=\"" + location
                 + "\">here</a>.</p></body></html>" );
}

int ft::Response::checkRedirect()
{
    for ( std::vector<Route>::iterator it = server_conf.routes.begin();
          it != server_conf.routes.end();
          ++it ) {
        if ( it->location_dir == request.uri ) {
            if ( it->http_redirection.size() > 0 ) {
                setStatusCode( status_codes.getStatusCode( 301 ) );
                setLocation( it->http_redirection[0] );
                // body with a script to redirect to the new location
                setContentType( mime_types.getMimeType( ".html" ) );
                makeRedirectFile( body, it->http_redirection[0] );
                return TRUE;
            }
        }
    }
    return FALSE;
}

void ft::Response::handleGet()
{
    if ( checkRedirect() ) {
        return;
    }

    if ( isDirectory( server_conf.root_dir + request.uri ) ) {
        if ( request.uri == "/" ) {
            setContentType( mime_types.getMimeType( ".html" ) );
            readFromAFile( getPath( request.uri ), body );
            setStatusCode( status_codes.getStatusCode( 200 ) );
        } else if ( canAutoIndex( request.uri ) ) {
            setContentType( mime_types.getMimeType( ".html" ) );
            createDirectoryListingIntoHTML( getPath( request.uri ), body );
            setBody( body );
            setStatusCode( status_codes.getStatusCode( 200 ) );
        } else {
            callErrorPage( body, server_conf.error_page[1] );
        }
    } else if ( isFile( getPath( request.uri ) ) ) {
        readFromAFile( getPath( request.uri ), body );
        setContentType( mime_types.getMimeType(
            "." + getFileExtension( getPath( request.uri ) ) ) );
        setStatusCode( status_codes.getStatusCode( 200 ) );
    } else {
        callErrorPage( body, server_conf.error_page[1] );
    }
}
