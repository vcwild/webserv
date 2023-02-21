#include "cgi_handler.hpp"
#include "parsers.hpp"
#include "server.hpp"
#include "webserv.hpp"
#include <csignal>
#include <iostream>

void sigint_handler( int sig )
{
    logger.info( "SIGINT received " + NumberToString( sig ) );
    exit( -1 );
}

int main( int argc, char **argv )
{
    if ( argc > 5 )
        std::cout << "Error" << std::endl;

    if ( argc == 1 ) {
        string path = argv[0];
        if ( path == "./bin/webserv" )
            argv[1] = ( char * ) "./config/default.conf";
    }

    std::ifstream test( argv[1] );
    if ( !test ) {
        logger.error( "Error: invalid path" );
        return 1;
    }

    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = sigint_handler;
    sigemptyset( &sigIntHandler.sa_mask );
    sigIntHandler.sa_flags = 0;
    sigaction( SIGINT, &sigIntHandler, NULL );

    logger.info( "Webserv starting" );

    vector<Config> conf = parseConfig( argv[1] );

    std::string configPath = argv[1];
    logger.info( "Config parsed from file: " + configPath );

    Server server( conf );

    return 0;
}
