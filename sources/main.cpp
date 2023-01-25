#include "Cgi_handler.hpp"
#include "Server.hpp"
#include "parsers.hpp"
#include "webserv.hpp"
#include <iostream>

int main( int argc, char **argv )
{
    if ( argc > 5 )
        std::cout << "Error" << std::endl;

    std::ifstream test( argv[1] );
    if ( !test ) {
        std::cout << "Error: invalid path" << std::endl;
        return 1;
    }

    vector<Config> conf = parseConfig( argv[1] );
    Server         server( conf );
    return 0;
}
