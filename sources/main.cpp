#include "Cgi_handler.hpp"
#include "parsers.hpp"
#include "server.hpp"
#include "webserv.hpp"
#include <iostream>

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
        std::cout << "Error: invalid path" << std::endl;
        return 1;
    }

    vector<Config> conf = parseConfig( argv[1] );
    Server         server( conf );
    return 0;
}
