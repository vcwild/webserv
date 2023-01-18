#ifndef WEBSERV_H
#define WEBSERV_H

#include "parsers.hpp"
#include <sstream>
#include <string>
#include <vector>

/* Definitions */
#define LISTEN_PORT_KEY "listen"
#define SERVER_NAME_KEY "server_name"
#define CLIENT_MAX_BODY_SIZE_KEY "client_max_body_size"
#define INDEX_KEY "index"
#define ALLOWED_METHOD_KEY "allowed_method"
#define ERROR_PAGE_KEY "error_page"
#define AUTOINDEX_KEY "autoindex"
#define ROUTE_KEY "location"
#define ROOT_KEY "root"
#define CGI_KEY "cgi"
#define HTTP_REDIRECTION_KEY "http_redirection"
#define ALLOW_METODS_KEY "allow_methods"

/* Structs */
struct Route
{
    std::vector<std::string> allowed_methods;
    std::string              root;
    std::string              index;
    std::vector<std::string> cgi;
    std::string              location_dir;
    int                      client_max_body_size;
    std::vector<std::string> http_redirection;
    std::string              autoindex; // directory listing
};

struct Config
{
    int                      listen_port;
    std::string              server_name;
    int                      client_max_body_size;
    std::vector<std::string> index;
    std::vector<std::string> allowed_method;
    std::vector<std::string> error_page;
    std::string              autoindex;
    std::vector<Route>       routes;
    std::string              root_dir;

    Config()
    {
        listen_port          = 0;
        server_name          = "";
        client_max_body_size = 0;
        index                = std::vector<std::string>();
        allowed_method       = std::vector<std::string>();
        error_page           = std::vector<std::string>();
        autoindex            = "";
        routes               = std::vector<Route>();
        root_dir             = "";
    };

    Config( std::vector<std::string>::iterator &it,
            std::vector<std::string>::iterator &end )
    {
        listen_port          = 0;
        server_name          = "";
        client_max_body_size = 0;
        index                = std::vector<std::string>();
        allowed_method       = std::vector<std::string>();
        error_page           = std::vector<std::string>();
        autoindex            = "";
        routes               = std::vector<Route>();
        root_dir             = "";

        while ( it != end ) {
            if ( ( *it ).find( "location " ) != std::string::npos ) {
                std::vector<std::string >              tmpVec( it, end );
                std::vector<std::vector<std::string> > tmpRoutes
                    = splitLines( tmpVec, ROUTE_KEY, true );

                std::vector<std::vector<std::string> >::iterator it2
                    = tmpRoutes.begin();

                while ( it2 != tmpRoutes.end() ) {
                    std::vector<std::string>::iterator it3 = it2->begin();
                    Route                              route;

                    while ( it3 != it2->end() ) {
                        setRouteAttributes( route, *it3 );
                        ++it3;
                    }
                    route.client_max_body_size = client_max_body_size;
                    route.autoindex            = autoindex;
                    routes.push_back( route );
                    ++it2;
                }
            }
            setConfig( *it );
            ++it;
        }
    };

    void setRouteAttributes( Route &route, std::string &line )
    {
        if ( ( line ).find( ROOT_KEY ) != std::string::npos ) {
            route.root = handleKey( line );
        } else if ( ( line ).find( INDEX_KEY ) != std::string::npos ) {
            route.index = handleKey( line );
        } else if ( ( line ).find( CGI_KEY ) != std::string::npos ) {
            route.cgi = handleVectorKey( line );
        } else if ( ( line ).find( CLIENT_MAX_BODY_SIZE_KEY )
                    != std::string::npos ) {
            std::istringstream( handleKey( line ) )
                >> route.client_max_body_size;
        } else if ( ( line ).find( HTTP_REDIRECTION_KEY )
                    != std::string::npos ) {
            route.http_redirection = handleVectorKey( line );
        } else if ( ( line ).find( ALLOW_METODS_KEY ) != std::string::npos ) {
            route.allowed_methods = handleVectorKey( line );
        } else if ( ( line ).find( ROUTE_KEY ) != std::string::npos ) {
            std::string tmp = handleKey( line );

            tmp                = tmp.substr( 0, tmp.size() - 2 );
            route.location_dir = tmp;
        }
    }

    void setConfig( std::string &line )
    {
        if ( line.find( LISTEN_PORT_KEY ) != std::string::npos ) {
            std::istringstream( handleKey( line ) ) >> listen_port;
        } else if ( line.find( SERVER_NAME_KEY ) != std::string::npos ) {
            server_name = handleKey( line );
        } else if ( line.find( CLIENT_MAX_BODY_SIZE_KEY )
                    != std::string::npos ) {
            istringstream( handleKey( line ) ) >> client_max_body_size;
        } else if ( line.find( AUTOINDEX_KEY ) != std::string::npos ) {
            autoindex = handleKey( line );

        } else if ( ( index.size() <= 0 )
                    && line.find( INDEX_KEY ) != std::string::npos ) {
            index = handleVectorKey( line );
        } else if ( line.find( ALLOWED_METHOD_KEY ) != std::string::npos ) {
            allowed_method = handleVectorKey( line );
        } else if ( line.find( ERROR_PAGE_KEY ) != std::string::npos ) {
            error_page = handleVectorKey( line );
        } else if ( line.find( ROOT_KEY ) != std::string::npos ) {
            root_dir = handleKey( line );
        }
    };
};

struct Request
{
    std::string method; // GET
    std::string host; // hostname
    std::string port; // port
    std::string uri; // /index.html
    std::string version; // HTTP/1.1
    std::string body; // request body
    std::string content_type; // text/html
    std::string user_agent; // Mozilla/5.0 (X11; Linux x86_64; rv:78.0)
    std::string authorization;
    std::string query;
    std::string cgi_path;
};

struct Response
{
    std::string version; // HTTP/1.1
    std::string status_code; // 200
    std::string status_message; // OK
    std::string content_type; // text/html
    std::string content_length; // 1234
    std::string body;
};

#endif
