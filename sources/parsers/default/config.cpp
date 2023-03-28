#include "webserv.hpp"

Config::Config()
{
    listen_port          = 0;
    server_name          = "";
    client_max_body_size = -1;
    index                = std::vector<std::string>();
    allowed_method       = std::vector<std::string>();
    error_page           = std::vector<std::string>();
    autoindex            = "";
    routes               = std::vector<Route>();
    root_dir             = "";
}

Config::Config( std::vector<std::string>::iterator &it,
                std::vector<std::string>::iterator &end )
{
    listen_port          = 0;
    server_name          = "";
    client_max_body_size = -1;
    index                = std::vector<std::string>();
    allowed_method       = std::vector<std::string>();
    error_page           = std::vector<std::string>();
    autoindex            = "";
    routes               = std::vector<Route>();
    root_dir             = "";

    while ( it != end ) {
        if ( ( *it ).find( "location " ) != std::string::npos
             && routes.size() <= 0 ) {
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
                routes.push_back( route );

                ++it2;
            }
        }
        setConfig( *it );
        ++it;
    }
}

void Config::setRouteAttributes( Route &route, std::string &line )
{
    if ( ( line ).find( ROOT_KEY ) != std::string::npos ) {
        route.root = handleKey( line );
    } else if ( ( line ).find( AUTOINDEX_KEY ) != std::string::npos ) {
        route.autoindex = handleKey( line );
    } else if ( ( line ).find( INDEX_KEY ) != std::string::npos ) {
        route.index = handleKey( line );
    } else if ( ( line ).find( CGI_KEY ) != std::string::npos ) {
        route.cgi = handleVectorKey( line );
    } else if ( ( line ).find( CLIENT_MAX_BODY_SIZE_KEY )
                != std::string::npos ) {
        std::istringstream( handleKey( line ) ) >> route.client_max_body_size;
    } else if ( ( line ).find( HTTP_REDIRECTION_KEY ) != std::string::npos ) {
        route.http_redirection = handleVectorKey( line );
    } else if ( ( line ).find( ALLOW_METODS_KEY ) != std::string::npos ) {
        route.allow_methods = handleVectorKey( line );
    } else if ( ( line ).find( ROUTE_KEY ) != std::string::npos ) {
        std::string tmp = handleKey( line );

        tmp                = tmp.substr( 0, tmp.size() - 2 );
        route.location_dir = tmp;
    }
}

void Config::setConfig( std::string &line )
{
    if ( line.find( LISTEN_PORT_KEY ) != std::string::npos ) {
        std::istringstream( handleKey( line ) ) >> listen_port;
    } else if ( line.find( SERVER_NAME_KEY ) != std::string::npos ) {
        server_name = handleKey( line );
    } else if ( line.find( CLIENT_MAX_BODY_SIZE_KEY ) != std::string::npos ) {
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
}
